//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_BACKUPPROCESS_H__21631B9D_4475_481F_BE79_37457E68C0D0__INCLUDED_)
#define AFX_BACKUPPROCESS_H__21631B9D_4475_481F_BE79_37457E68C0D0__INCLUDED_

#include <string>
#include <vector>

using namespace std;

struct BackupFolders
{
    string  folder;
    string  filter;
    bool    chdate;
};

class Ftp_Client;
class BackupProcess  
{
public:
	BackupProcess();
	virtual     ~BackupProcess();
    void        Start(HWND h=0);
    void        Stop();
    void        Break(BOOL wait=1);
    static int  BP(void*);
    void        Clear();
    void        LogInFile(LPCTSTR p, ...);
    void        LogFileAssociations(LPCTSTR p,...);
    BOOL        BackupOnFTP();
    void        BackupOnDrive();
    int         FTP_CopyFiles(BackupFolders& folder, Ftp_Client& fc);
    void        ProcessFile(Ftp_Client& fc, WIN32_FIND_DATA& fData);
    int         FileExist(WIN32_FIND_DATA& fDataLoco, const vector<WIN32_FIND_DATA>& remoteFiles, WIN32_FIND_DATA* itFind);
    BOOL        ResolveRemoteDir(Ftp_Client& fc, TCHAR* p=0);
    void        R_CopyFiles(Ftp_Client& fc);
    BOOL        IsWorking(){return _bThreadOn;};
    BOOL        ResolveFullSubDirs(Ftp_Client& fc, TCHAR* srcDir, TCHAR* dstDir);
    BOOL        ResolveFTPSubDirs(Ftp_Client& fc, TCHAR* srcDir, TCHAR* dstDir);

    Ftp_Client*      _pFc;
    BOOL             _stopping;
    BOOL             _hStopBackup;
    HANDLE           _hThread;
    FILE*            _pfLog;
    FILE*            _pfLogAssoc;
    CString          _instExtension;
    TCHAR            _locoPathIni[_MAX_PATH];
    TCHAR            _locoPath[_MAX_PATH];
    TCHAR            _remPath[_MAX_PATH];
    BOOL             _errors;
    BOOL             _files;
    BOOL             _bThreadOn;
    BOOL             _settingsOn;
    HWND             _hLogWindow;
    BOOL             _laterClear;

    UINT             _nTotalFiles;
    UINT             _nCopyedFiles;
    BOOL             _rescandirs;
};

#endif // !defined(AFX_BACKUPPROCESS_H__21631B9D_4475_481F_BE79_37457E68C0D0__INCLUDED_)
