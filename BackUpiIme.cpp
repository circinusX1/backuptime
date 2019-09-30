//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include <direct.h>
#include "BackUpiIme.h"
#include "BackUpiImeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBackUpiImeApp, CWinApp)
	//{{AFX_MSG_MAP(CBackUpiImeApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CBackUpiImeApp::CBackUpiImeApp()
{
    _working     = 0;
    _wndMain     = 0;
    _adminMode   = 0;
    _enabled     = 1;
    _settingsOn  = 0;
}

//-----------------------------------------------------------------------------
CBackUpiImeApp theApp;

//-----------------------------------------------------------------------------
static void Encode(BYTE* pd,  int ln)
{
	for(int i=0;i<ln;i++)
		*(pd+i)=*(pd+i)^0x5a;
}

//-----------------------------------------------------------------------------
static void Decode(BYTE* pd,  int ln)
{
	for(int i=0;i<ln;i++)
		*(pd+i)=*(pd+i)^0x5a;
}

//-----------------------------------------------------------------------------
BOOL CBackUpiImeApp::InitInstance()
{
	AfxEnableControlContainer();

    _getcwd(_appdir,_MAX_PATH);
    _adminMode = 0;

    TCHAR userName[256]={0};
    DWORD nl = 255;
    GetUserName(userName,  &nl);

    SECURITY_ATTRIBUTES  sa;
    SECURITY_DESCRIPTOR  sd;
    sa.nLength          = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle   = TRUE;
    sa.lpSecurityDescriptor = &sd;
   
    if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION)) 
    {
        AfxMessageBox("unable to InitializeSecurityDescriptor, err = %d",GetLastError());
        return FALSE;
    }

    if(!SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE)) 
    {
        AfxMessageBox("unable to SetSecurityDescriptorDacl, err = %d",GetLastError());
        return FALSE;
    }

    _adminMode = IsAdmin();

    CString         entry;
    CString         files;
    CString         folder;
    BackupFolders   bcf;

    UINT    folders = theApp.GetProfileInt("FOLDERS","COUNT",0);
    for(UINT i=0;i<folders;i++)
    {
        entry.Format("FOLDER%d",i);
        folder = theApp.GetProfileString("FOLDERS",entry);
        
        entry.Format("FILES%d",i);
        files = theApp.GetProfileString("FOLDERS",entry);

        if(files.GetLength() > 1)
        {
            bcf.folder = (LPCTSTR)folder;
            bcf.filter = (LPCTSTR)files;
            _vFolders.push_back(bcf);
        }
    }

   _isFtp            = GetProfileInt("FTP","FTP_ENABLED", 1);
   _backupLocation   = GetProfileString("FTP","LOCATION","/");
   _allwaysOverwrite = GetProfileInt("FTP","OVERFILE",1);
   _logfiles         = GetProfileInt("FTP","FTP_GENLOG",0);
   _ftpIP            = GetProfileString("FTP","IP","ftp://localhost");
   _ftpPort          = GetProfileInt("FTP","PORT",21);
   _ftpUser          = GetProfileString("FTP","USER","anonymous" );
   _ftpPasssword     = GetProfileString("FTP","PASSWORD","");
   _remoteFolder   = GetProfileString("FTP","REMOTEFOLDER","");
   m_treestruct    = GetProfileInt("FTP","TREESTRUCT",1);
    _month2Run  = GetProfileInt("SETT","MONTH",    -1); // -1 every month
    _hour2Run   = GetProfileInt("SETT","HOUR",     0);  // 0 23
    _dayOfWeek  = GetProfileInt("SETT","DAY",      -1); // (-1 every day)
    b_showerrormsgbox = GetProfileInt("SETT","ERROR_ON",  0);
    b_workinbackground = GetProfileInt("SETT","BACKGROUND",   1);
    b_taskbar      = GetProfileInt("SETT","TRAYICON",  1);
    b_logerrors    = GetProfileInt("SETT","LOG_ERRORS", 1);
    _priority      = GetProfileInt("SETT","PRIORITY",   1);
    b_changeft     = GetProfileInt("SETT","FILETIME" ,  1);
    BOOL ft =  GetProfileInt("APP","INITIALIZATION", 0);
    if(0==ft)
    {
        WriteProfileInt("APP","INITIALIZATION", 1);
        _enabled = 0;
    }
    static const TCHAR* psz = ::AfxRegisterWndClass(0,0,0,0);
    _wndMain = new CFrm();
    _wndMain->CreateEx(WS_EX_TOOLWINDOW,psz,"",WS_OVERLAPPEDWINDOW,0,0,0,0,0,0);
    m_pMainWnd = _wndMain;
    m_pMainWnd->ShowWindow(SW_HIDE);

    if(_enabled==0)
    {
        m_pMainWnd->PostMessage(WM_COMMAND,MN_SETTINGS);
    }
	return 1;
}



int CBackUpiImeApp::ExitInstance() 
{   
    CString entry;
    int     i = 0;

    back_process.Stop();
    SaveSettings();
	return CWinApp::ExitInstance();
}


void CBackUpiImeApp::SaveSettings()
{
    CString entry;
    int     i=0;

    theApp.WriteProfileInt("FOLDERS","COUNT",_vFolders.size());
    vector<BackupFolders>::iterator b = theApp._vFolders.begin();
    vector<BackupFolders>::iterator e = theApp._vFolders.end();
    for(; b!=e; b++)
    {
        entry.Format("FOLDER%d",i);
        theApp.WriteProfileString("FOLDERS",entry,b->folder.c_str());
        
        entry.Format("FILES%d",i);
        theApp.WriteProfileString("FOLDERS",entry,b->filter.c_str());


        ++i;
    }
    
    for(;i<32;i++)
    {
        entry.Format("FOLDER%d",i);
        theApp.WriteProfileString("FOLDERS",entry,"");
        
        entry.Format("FILES%d",i);
        theApp.WriteProfileString("FOLDERS",entry,"");
        ++i;
    }
    
    
    WriteProfileInt("FTP","FTP_ENABLED",_isFtp           );
    WriteProfileString("FTP","LOCATION",_backupLocation  );
    WriteProfileInt("FTP","OVERFILE",_allwaysOverwrite     );
    WriteProfileInt("FTP","FTP_GENLOG",_logfiles         );
    
    
    WriteProfileString("FTP","IP",_ftpIP           );
    WriteProfileInt("FTP","PORT",_ftpPort         );
    WriteProfileString("FTP","USER",_ftpUser         );
    WriteProfileString("FTP","PASSWORD",_ftpPasssword    );
    WriteProfileString("FTP","REMOTEFOLDER",_remoteFolder);
    WriteProfileInt("FTP","TREESTRUCT",m_treestruct);
    
    WriteProfileInt("SETT","MONTH",    _month2Run); // -1 every month
    WriteProfileInt("SETT","HOUR",     _hour2Run);  // 0 23
    WriteProfileInt("SETT","DAY",      _dayOfWeek); // (-1 every day)
    WriteProfileInt("SETT","ERROR_ON",  b_showerrormsgbox);
    WriteProfileInt("SETT","BACKGROUND",   b_workinbackground);
    WriteProfileInt("SETT","TRAYICON",  b_taskbar);
    WriteProfileInt("SETT","LOG_ERRORS", b_logerrors);
    WriteProfileInt("SETT","PRIORITY" ,  _priority);
    WriteProfileInt("SETT","FILETIME" ,  b_changeft);
    
}

void CBackUpiImeApp::CheckTimer(UINT donow)
{
    #define E_VERY  (-1)
    if(_working)
    {
        ////Beep(400,10);
        return;
    }
    if(donow!=0)
    {
        
        StartBackupProcess((HWND)donow);
        return;
    }
    if(theApp._settingsOn)
    {
        ////Beep(400,10);
        return;
    }

    GetLocalTime(&_currBackupTime);
    _currBackupTime.wMinute       = 0; //only hours that matters only
    _currBackupTime.wMilliseconds = 0;
    _currBackupTime.wSecond       = 0;
    
    // we've cross a hour
    if(!memcmp(&_lastBackupTime,&_currBackupTime,sizeof(SYSTEMTIME)))
    {
        return;
    }

    SYSTEMTIME    st;
    GetLocalTime(&st);

    if(_month2Run == E_VERY)
    {
        if(_dayOfWeek == E_VERY)
        {
            if(_hour2Run == E_VERY)
            {
                _working = 1;
                StartBackupProcess();
                return;
            }
            else if(_hour2Run == st.wHour)
            {
                StartBackupProcess();
                return;
            }
        }
        else if(_dayOfWeek == st.wDayOfWeek)
        {
            if(_hour2Run == E_VERY)
            {
                StartBackupProcess();
                return;
            }
            else if(_hour2Run == st.wHour)
            {
                StartBackupProcess();
                return;
            }
        }
    }
    else if(_month2Run == st.wMonth)
    {
        if(_dayOfWeek == E_VERY)
        {
            if(_hour2Run == E_VERY)
            {
                StartBackupProcess();
                return;
            }
            else if(_hour2Run == st.wHour)
            {
                StartBackupProcess();
                return;
            }
        }
        else if(_dayOfWeek == st.wDayOfWeek)
        {
            if(_hour2Run == E_VERY)
            {
                StartBackupProcess();
                return;
            }
            else if(_hour2Run == st.wHour)
            {
                StartBackupProcess();
                return;
            }
        }
    }
}

void CBackUpiImeApp::StartBackupProcess(HWND h)
{
    _working = 1;
    if(_currBackupTime.wYear==0)
    {
        GetLocalTime(&_currBackupTime);
    }
    _lastBackupTime = _currBackupTime;
    back_process.Start(h);
}

BOOL CBackUpiImeApp::IsAdmin(void) 
{

   HANDLE hToken;
   DWORD  dwStatus;
   DWORD  dwAccessMask;
   DWORD  dwAccessDesired;
   DWORD  dwACLSize;
   DWORD  dwStructureSize = sizeof(PRIVILEGE_SET);
   PACL   pACL            = NULL;
   PSID   psidAdmin       = NULL;
   BOOL   bReturn         = FALSE;

   PRIVILEGE_SET   ps;
   GENERIC_MAPPING GenericMapping;

   PSECURITY_DESCRIPTOR     psdAdmin           = NULL;
   SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;
   
   __try {

      // AccessCheck() requires an impersonation token.
      ImpersonateSelf(SecurityImpersonation);

      if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, 
            &hToken)) {

         if (GetLastError() != ERROR_NO_TOKEN)
            __leave;

         // If the thread does not have an access token, we'll 
         // examine the access token associated with the process.
         if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, 
               &hToken))
            __leave;
      }

      if (!AllocateAndInitializeSid(&SystemSidAuthority, 2, 
            SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0, &psidAdmin))
         __leave;

      psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
      if (psdAdmin == NULL)
         __leave;

      if (!InitializeSecurityDescriptor(psdAdmin,
            SECURITY_DESCRIPTOR_REVISION))
         __leave;
  
      // Compute size needed for the ACL.
      dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) +
            GetLengthSid(psidAdmin) - sizeof(DWORD);

      // Allocate memory for ACL.
      pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
      if (pACL == NULL)
         __leave;

      // Initialize the new ACL.
      if (!InitializeAcl(pACL, dwACLSize, ACL_REVISION2))
         __leave;

      dwAccessMask= 1 | 2;
      
      // Add the access-allowed ACE to the DACL.
      if (!AddAccessAllowedAce(pACL, ACL_REVISION2,
            dwAccessMask, psidAdmin))
         __leave;

      // Set our DACL to the SD.
      if (!SetSecurityDescriptorDacl(psdAdmin, TRUE, pACL, FALSE))
         __leave;

      // AccessCheck is sensitive about what is in the SD; set
      // the group and owner.
      SetSecurityDescriptorGroup(psdAdmin, psidAdmin, FALSE);
      SetSecurityDescriptorOwner(psdAdmin, psidAdmin, FALSE);

      if (!IsValidSecurityDescriptor(psdAdmin))
         __leave;

      dwAccessDesired = 1;

      // 
      // Initialize GenericMapping structure even though we
      // won't be using generic rights.
      // 
      GenericMapping.GenericRead    = 1;
      GenericMapping.GenericWrite   = 2;
      GenericMapping.GenericExecute = 0;
      GenericMapping.GenericAll     = 1 | 2;

      if (!AccessCheck(psdAdmin, hToken, dwAccessDesired, 
            &GenericMapping, &ps, &dwStructureSize, &dwStatus, 
            &bReturn)) {
         printf("AccessCheck() failed with error %lu\n", GetLastError());
         __leave;
      }

      RevertToSelf();
   
   } __finally {

      // Cleanup 
      if (pACL) LocalFree(pACL);
      if (psdAdmin) LocalFree(psdAdmin);  
      if (psidAdmin) FreeSid(psidAdmin);
   }

   return bReturn;
}
