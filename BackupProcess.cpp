//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include <direct.h>
#include "backupiime.h"
#include "BackupProcess.h"
#include "Ftp_Client.h"

BackupProcess::BackupProcess()
{
    _pFc         = 0;
    _stopping    = 0;
    _hStopBackup = 0;
    _hThread     = 0;
    _hLogWindow  = 0;
    _laterClear  = 0;
    _rescandirs  = 0;  
}

BackupProcess::~BackupProcess()
{
}

void BackupProcess::Clear()
{
    _hLogWindow = 0;
    _laterClear=1;
}


void        BackupProcess::Stop()
{
    _hLogWindow = 0;
    _stopping=1;
    if(_hThread)
    {
        if(_pFc)
        {
            _pFc->SetLogOutWindow(0);
        }

        _hStopBackup = 1;       // syncronisation in this casse is not an issue
        Sleep(4000);
        while(_bThreadOn)        // syncronisation in this casse is not an issue   
        {
            if(WAIT_OBJECT_0==WaitForSingleObject(_hThread,100))
                break;
            theApp.PumpMessage();// we have a SendMessage From Thread
            Sleep(10);
        }
        ::CloseHandle(_hThread);
    }
    _hThread    =0;
    _hStopBackup=0;
    _stopping   =0;
}

void   BackupProcess::Break(BOOL bwait)
{
    _hLogWindow = 0;
    _stopping=1;
    if(_hThread)
    {
        if(_pFc)
        {
            _pFc->SetLogOutWindow(0);
        }
        _hStopBackup=1;             // syncronisation in this casse is not an issue
        if(bwait)
        {
            if(_bThreadOn)
            {
                Sleep(1000);
                while(_bThreadOn)    // syncronisation in this casse is not an issue
                {
                    if(WAIT_OBJECT_0==WaitForSingleObject(_hThread,100))
                        break;
                    theApp.PumpMessage(); // we have a SendMessage From Thread
                    Sleep(10);
                }
            }
            ::CloseHandle(_hThread);
        }
        _hThread = 0;
    }
    _stopping=0;
}

void BackupProcess::Start(HWND h)
{
    if(_stopping)
        return;
    if(_laterClear)
    {
        _laterClear  = 0;
    }
    _nTotalFiles = 0;
    _nCopyedFiles = 0;
    _hStopBackup = 0;
    _hLogWindow  = h;
    if(_hThread == 0)
    {
        DWORD dwid;
        _hThread = ::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)BP, this, 0, &dwid);
        ::SetThreadPriority(_hThread, -4);
        Sleep(100);
    }
}

int BackupProcess::BP(void* nothing)
{
    TRACE("Backuping \r\n");

    BackupProcess* pBp = (BackupProcess*)nothing;
    TCHAR f1[_MAX_PATH];
    TCHAR f2[_MAX_PATH];

    pBp->_bThreadOn   = 1;
    pBp->_errors = 0;
    pBp->_files  = 0;

    for(int i=0; i<2; i++){
        theApp._wndMain->PostMessage(WM_WORKING,1,0);
        Sleep(1);
    }
    
    if(theApp.b_logerrors)
    {
        SYSTEMTIME st; 
        GetLocalTime(&st);
        
        chdir(theApp._appdir);
        sprintf(f1,"btlog-%02d-%02d.log",st.wMonth, st.wDay);
        sprintf(f2,"btmap-%02d-%02d.log",st.wMonth, st.wDay);
        pBp->_pfLog      = ::fopen(f1,"ab");
        pBp->_pfLogAssoc = ::fopen(f2,"ab");
    }

    try{
        
        if(theApp._isFtp)
        {
            for(int i=0;i<5;i++)
            {
                if(pBp->BackupOnFTP())
                {
                    break;
                }
                Sleep(5000); //wait for ftp to wake up
            }
        }
        else if(theApp._remoteFolder.GetLength())
        {
            pBp->BackupOnDrive();
        }
        if(!theApp._isFtp)
        {
            pBp->LogInFile("+Neither FTP or Remote Drive Is configured !\r\n");
        }
    }
    catch(...)
    {
        pBp->LogInFile("+Unespected Exception. BP()\r\n");
    }
    if(theApp.b_logerrors)
    {
        if(pBp->_pfLog)      
            ::fclose(pBp->_pfLog);
        if(pBp->_pfLogAssoc) 
            ::fclose(pBp->_pfLogAssoc);
        Sleep(12);
        if(!pBp->_errors)
        {
            ::_unlink(f1);
        }
        if(!pBp->_files)
        {
            ::_unlink(f2);
        }
    }
    TRACE("Done \r\n");
    pBp->_bThreadOn   = 0; // not to critical
    for(int i=0;i<4;i++)theApp._wndMain->PostMessage(WM_WORKING,0,0);
    return 0;
}

BOOL BackupProcess::BackupOnFTP()
{
    BOOL rv = TRUE;
    int errors  = 0;
    Ftp_Client  fc;

    _pFc = &fc;

    if(!fc.OpenSession("Backup-Thread", _hLogWindow, PRE_CONFIG_INTERNET_ACCESS))
    {
        LogInFile("+Cannot Open Net Session: Error %d-%d\r\n", fc.m_dwLastRespInfo, GetLastError());
        errors =1;
        rv = FALSE;
        goto EXITFOO;
    }
    if(!fc.Connect(theApp._ftpIP, theApp._ftpUser, theApp._ftpPasssword, theApp._ftpPort, theApp._passsiveMode))
    {
        LogInFile("+Cannot Connect To '%s' Error %d-%d\r\n", theApp._ftpIP, fc.m_dwLastRespInfo, GetLastError());
        errors =1;
        rv = FALSE;
        goto EXITFOO;
    }
    if(!fc.SetCurrentDirectory(theApp._backupLocation))
    {
        if(!fc.CreateDirectory(theApp._backupLocation))
        {
            LogInFile("+Cannot Create Directory '%s' Error: %d",theApp._backupLocation, fc.m_dwLastRespInfo);
            errors =1;
            rv = FALSE;
            goto EXITFOO;
        }
        if(!fc.SetCurrentDirectory(theApp._backupLocation))
        {
            LogInFile("+Cannot Change Directory to '%s' Error: %d",theApp._backupLocation, fc.m_dwLastRespInfo);
            errors =1;
            rv = FALSE;
            goto EXITFOO;
        }
    }
    ::_tcscpy(_remPath, theApp._backupLocation);
    do{
        vector<BackupFolders>::iterator folderB = theApp._vFolders.begin();
        vector<BackupFolders>::iterator folderE = theApp._vFolders.end();
        for(;folderB!=folderE;folderB++)
        {
            BackupFolders& folder = *folderB;
            errors = FTP_CopyFiles(folder, fc);
            if(_hStopBackup)    // syncronisation in this casse is not an issue
            {
                break;
            }
        }
    } while(0);

EXITFOO:
    _pFc = 0;
    fc.Close();
    return rv;
}

int BackupProcess::FTP_CopyFiles(BackupFolders& folder, Ftp_Client& fc)
{
    _rescandirs  = 1;
    if(-1==::_chdir(folder.folder.c_str()))
    {
        LogInFile("+Path: '%s' cannot be found\r\n", folder.folder.c_str());
        return 1;
    }
    
    ::_getcwd(_locoPathIni,_MAX_PATH);
    if(_locoPathIni[_tcslen(_locoPathIni)-1]!='\\')    
        _tcscat(_locoPathIni,"\\");

    ::_getcwd(_locoPath,_MAX_PATH);
    if(!ResolveRemoteDir(fc, 0))
    {
        return 1;
    }
    
    _instExtension = folder.filter.c_str();
    _instExtension.Replace("*","");
    R_CopyFiles(fc);
    return 0;
}


void BackupProcess::R_CopyFiles(Ftp_Client& fc)
{
    if(_hStopBackup)    // syncronisation in this casse is not an issue
        return;
    char*           extension;
	HANDLE			hfFind;
	WIN32_FIND_DATA	fDATA;	
    TCHAR           cwd[260];

    ::_getcwd(cwd, 260);


	if((hfFind=FindFirstFile("*", &fDATA)) != INVALID_HANDLE_VALUE)
	{
		if(fDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(fDATA.cFileName[0]!='.')
			{
                _rescandirs = TRUE;
                ::_chdir(fDATA.cFileName);
                R_CopyFiles(fc);
				::_chdir("..");
                _rescandirs = TRUE;
			}
		}
        else if(fDATA.cFileName[0]!='.')
        {
            extension = strchr(fDATA.cFileName,'.');
            if(_instExtension=="."||(extension && strstr(_instExtension,extension)))
            {
		        ProcessFile(fc, fDATA);
            }
        }
		while(FindNextFile(hfFind, &fDATA)!=0)
		{
            if(_hStopBackup)    // syncronisation in this casse is not an issue
                break;

			if(fDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(fDATA.cFileName[0]!='.')
				{
                    _rescandirs = TRUE;
					::_chdir(fDATA.cFileName);
					R_CopyFiles(fc);
                    ::_chdir("..");
                    _rescandirs = TRUE;
				}
			}
            else if(fDATA.cFileName[0]!='.')
            {
                extension = strchr(fDATA.cFileName,'.');
                if(_instExtension=="."||(extension && strstr(_instExtension,extension)))
                {
		            ProcessFile(fc, fDATA);
                }
            }
		}
	}
	FindClose(hfFind);  
}

BOOL BackupProcess::ResolveRemoteDir(Ftp_Client& fc, TCHAR* df)
{
    TCHAR  srcDir[260]; ::_tcscpy(srcDir,_locoPath);
    TCHAR  dstDir[260]; ::_tcscpy(dstDir,_remPath);
    int     repeat = 3;
    static TCHAR    lastSz[_MAX_PATH] = "";

   
    if(_tcscmp(lastSz, _remPath))
    {
        if(!fc.SetCurrentDirectory(_remPath))
        {
            LogInFile("+Cannot Change Directory to '%s' Error: %d",_remPath, fc.m_dwLastRespInfo);
            return 0;
        }
        strcpy(lastSz,_remPath);
    }

    if(dstDir[_tcslen(dstDir)-1]!='/')    
    {
        _tcscat(dstDir,"/");
    }
    int j=0;
    char lastch = 0;
    char two[2] = {0};
    while(srcDir[j]!=0)
    {
        if(!theApp.m_treestruct)
        {
            if(srcDir[j]==_locoPathIni[j])
            {
                ++j;
                continue;
            }
        }


        if(srcDir[j]==':' || srcDir[j]=='\\')
        {
            if(!fc.SetCurrentDirectory(dstDir))
            {
                if(!fc.CreateDirectory(dstDir))
                {
                    LogInFile("+Cannot Create Directory '%s' Error: %d",dstDir, fc.m_dwLastRespInfo);
                    return 0;
                }
                if(!fc.SetCurrentDirectory(dstDir))
                {
                    LogInFile("+Cannot Change Directory to '%s' Error: %d",dstDir, fc.m_dwLastRespInfo);
                    return 0;
                }
            }
            _tcscat(dstDir,"/");
            if(srcDir[j]==':')
                j++;
            j++;
            continue;
        }

        two[0]=srcDir[j];
        _tcscat(dstDir, two);
        ++j;
    }

    if(!fc.SetCurrentDirectory(dstDir))
    {
        if(!fc.CreateDirectory(dstDir))
        {
            LogInFile("+Cannot Create Directory '%s' Error: %d",dstDir, fc.m_dwLastRespInfo);
            return 0;
        }
        if(!fc.SetCurrentDirectory(dstDir))
        {
            LogInFile("+Cannot Change Directory to '%s' Error: %d",dstDir, fc.m_dwLastRespInfo);
            return 0;
        }
    }
    if(_rescandirs)
    {
        fc.ListLocation(); // get latest files from over there
        _rescandirs=FALSE;
    }
    if(df)
        strcpy(df, dstDir);
    return 1;
}


int BackupProcess::FileExist(WIN32_FIND_DATA& fDataLoco, 
                             const vector<WIN32_FIND_DATA>& remoteFiles,
                             WIN32_FIND_DATA* itFind)
{
    SYSTEMTIME stLoco;
    SYSTEMTIME stRemote;

    vector<WIN32_FIND_DATA>::const_iterator b =  remoteFiles.begin();
    vector<WIN32_FIND_DATA>::const_iterator e =  remoteFiles.end();
    for(;b!=e;b++)
    {
        const WIN32_FIND_DATA& rfd = *b;

        if(!::stricmp(rfd.cFileName, fDataLoco.cFileName))
        {
            if(fDataLoco.ftLastWriteTime.dwHighDateTime == rfd.ftLastWriteTime.dwHighDateTime&&
               fDataLoco.ftLastWriteTime.dwLowDateTime == rfd.ftLastWriteTime.dwLowDateTime )
            {
                return 1;
            }
            return 0;
        }
    }
    return 0;
}


void    BackupProcess::ProcessFile(Ftp_Client& fc,
                                   WIN32_FIND_DATA& fData)
{
    static TCHAR s_fname[260] ="*";

    WIN32_FIND_DATA* itFind = 0;

    ::_getcwd(_locoPath,_MAX_PATH);
    
    if(!ResolveRemoteDir(fc, s_fname))
    {
        return;
    }

    _tcscat(s_fname, fData.cFileName);
    if(FileExist(fData, fc.v_lastList, itFind))
    {
        theApp._wndMain->PostMessage(WM_WORKING,2, (LPARAM)s_fname);    
        return;
    }
    _tcscat(s_fname," ***");
    theApp._wndMain->PostMessage(WM_WORKING,2, (LPARAM)s_fname);
    
    
    try{
        DWORD       d      = 0;
        int         repeat = 3;
        FILETIME    remFT={0};
        TCHAR       ftpcd[260]={0};
        DWORD       dwl = 260;
        ++_nTotalFiles;

        fc.p_FtpConnection->GetCurrentDirectory( ftpcd, &dwl);
        while(--repeat>0)
        {
            d = 0;
            if(fc.p_FtpConnection->PutFile(fData.cFileName, fData.cFileName))
            {
                Sleep(1);
                LogFileAssociations("%s\\%s=%s/%s",_locoPath, fData.cFileName, _remPath, fData.cFileName);
                ++_nCopyedFiles;    
                break;
            }
            d = GetLastError();
            if(d == 12003)
            {
                Sleep(64);
            }
            else
            {
                LogInFile("+Cannot Copy '%s\\%s' to '%s/%s'. Error %d",_locoPath,fData.cFileName,ftpcd,fData.cFileName, d);
                break;
            }
        }

        if(0==d && !strstr(_locoPath,"rofiles"))
        {
            SYSTEMTIME         st;
            /*
            char               mdtm[400];
            

            FileTimeToSystemTime(&fData.ftLastAccessTime, &st);
            FileTimeToSystemTime(&fData.ftCreationTime, &st);
            FileTimeToSystemTime(&fData.ftLastWriteTime, &st);
            sprintf(mdtm, "MDTM %04u%02hu%02hu%02hu%02hu%02hu %s",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, ftpcd,fData.cFileName);

            
            CInternetFile* pif = fc.p_FtpConnection->Command(mdtm, CFtpConnection::CmdRespRead);
            if(0 == pif)
            {
                DWORD dw = GetLastError(); 
                LogInFile("+Cannot Access File Time %s\\%s: Error %d",_locoPath,fData.cFileName, dw);

            }
            /*
            MDTM 2006 07 01 18 30 05 test.txt 
*/

            WIN32_FIND_DATA rfData;
       
            CFtpFileFind     ftpFind(fc.p_FtpConnection);

            memcpy(&rfData, &fData, sizeof(rfData));
            BOOL bContinue = ftpFind.FindFile(rfData.cFileName);
            if (bContinue)
            {
                WIN32_FIND_DATA dwd = {0};
                FILETIME           b = {0},c={0};
                
                while(bContinue)
                {
                    bContinue = ftpFind.FindNextFile();
                    CString& csn = ftpFind.GetFileName();
                    if(csn == rfData.cFileName)
                        break;
                }
                
                ftpFind.GetLastWriteTime(&b);
                ftpFind.Close();

                
                
                

                // change local time to remote time
                HANDLE hFile = CreateFile(MKSTR("%s\\%s",_locoPath,fData.cFileName), GENERIC_READ|GENERIC_WRITE ,
                                          FILE_SHARE_READ|FILE_SHARE_WRITE,
                                          NULL,
                                          OPEN_EXISTING,
                                          FILE_FLAG_NO_BUFFERING,
                                          NULL);

                if(hFile!=(HANDLE)-1)
                {
                    FileTimeToSystemTime(&b, &st); 
                    int f = SetFileTime(hFile, 0, 0, &b);
                    if(f==0)
                    {
                        DWORD dw = GetLastError(); 
                        LogInFile("+Cannot Set File Time for %s\\%s: Error %d",_locoPath,fData.cFileName, dw);

                    }
                    CloseHandle(hFile);
                }
                else
                {
                    DWORD dw = GetLastError(); 
                    LogInFile("+Cannot Access File Time %s\\%s: Error %d",_locoPath,fData.cFileName, dw);
                }
                

            }
            
        }
        

    }
    catch(...)
    {
        LogInFile("+Fatal Error While Copying '%s'",fData.cFileName);
    }

}


void BackupProcess::BackupOnDrive()
{
    int errors = 0;

    


//    ::CopyFile(srcDir, destfn, !_overwrite);
    // while copying check dates
}

void  BackupProcess::LogInFile(LPCTSTR p, ...)
{
    if(_hStopBackup)    return;
    if(_hLogWindow)
        goto GEE;


    if(theApp.b_logerrors==0)return;
    if(_pfLog == 0)return;
GEE:

    static TCHAR _s[1204];
    _errors=1;
    try{
	    va_list args;
        va_start(args, p);
        ::_vsnprintf(_s, sizeof(_s) / sizeof(TCHAR), p, args);
	    va_end(args);

        if(_hLogWindow)
        {
            ::SendMessage(_hLogWindow,WM_FTP_LOGTEXT,0,(long)_s);
        }
        if(theApp.b_logerrors==0)
            return;
        if(_pfLog == 0)
            return;

        ::fputs(_s, _pfLog);
        ::fputs("\r\n", _pfLog);
        TRACE(_s);
        TRACE("\r\n");


    }catch(...){}
}

void  BackupProcess::LogFileAssociations(LPCTSTR p, ...)
{
    if(_hStopBackup)    return;
    if(theApp.b_logerrors==0)return;
    if(_pfLogAssoc == 0)return;
    _files = 1;
    try{
        static TCHAR _s[1204];
    	va_list args;
        va_start(args, p);
        ::_vsnprintf(_s, sizeof(_s) / sizeof(TCHAR), p, args);
	    va_end(args);
        ::fputs(_s, _pfLogAssoc);
        ::fputs("\r\n", _pfLogAssoc);

        TRACE(_s);
        TRACE("\r\n");
    }catch(...){}
}


