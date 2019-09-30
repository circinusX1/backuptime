//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_BACKUPIIME_H__B92ACFAE_CE52_49FF_AC73_DDA4A72255F4__INCLUDED_)
#define AFX_BACKUPIIME_H__B92ACFAE_CE52_49FF_AC73_DDA4A72255F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <direct.h>
#include <afxinet.h>
#include "BackupProcess.h"
#include <string>
#include <vector>
#include <map>
#include "Ftp_Client.h"
#include "Frm.h"
using namespace std;

#define WM_WORKING (WM_USER+2000)

class CBackUpiImeApp : public CWinApp
{
public:
	CBackUpiImeApp();
    
    void CheckTimer(UINT donow);
    void StartBackupProcess(HWND h=0);
    void Redir(){_chdir(_appdir);}
    BOOL IsAdmin(void);
    void SaveSettings();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackUpiImeApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBackUpiImeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    vector<BackupFolders>   _vFolders;
    
    BOOL                    _isFtp;
    BOOL                    _logfiles;
    BOOL                    _allwaysOverwrite;
    CString                 _backupLocation;

    CString                 _ftpIP         ;
    UINT                    _ftpPort       ;
    CString                 _ftpUser       ;
    CString                 _ftpPasssword  ;
    BOOL                    _passsiveMode;
    CString                 _remoteFolder;
    

    UINT                    _month2Run; // -1 every month
    UINT                    _hour2Run;  // 0 23
    UINT                    _dayOfWeek; // (-1 every day)
    BOOL                    b_showerrormsgbox;
    BOOL                    b_workinbackground;
    BOOL                    b_taskbar;
    BOOL                    b_logerrors;
    BOOL                    b_changeft;
    UINT                    _priority;
    BOOL                    m_treestruct;

    SYSTEMTIME              _currBackupTime;
    SYSTEMTIME              _lastBackupTime;
    BOOL                    _working;

    CFrm*                   _wndMain;
    BOOL                    _adminMode;
    BOOL                    _enabled;

    BOOL                    _settingsOn;
    BackupProcess           back_process;
    TCHAR                   _appdir[_MAX_PATH];
};

extern CBackUpiImeApp theApp;


class Cfms
{
public:
	Cfms(char* p,...){
		va_list args;
		va_start(args, p);
        ::_vsnprintf(_s, sizeof(_s) / sizeof(TCHAR), p, args);
		va_end(args);
	}
	operator const TCHAR*(){return _s;}
	TCHAR _s[256];
};

#define	MKSTR	(const TCHAR*)Cfms


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKUPIIME_H__B92ACFAE_CE52_49FF_AC73_DDA4A72255F4__INCLUDED_)
