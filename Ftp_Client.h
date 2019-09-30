//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_FTP_CLIENT_H__06713E04_A9AD_4815_AEF6_62FB1462951B__INCLUDED_)
#define AFX_FTP_CLIENT_H__06713E04_A9AD_4815_AEF6_62FB1462951B__INCLUDED_

#include <vector>
#include <string>
#include <afxinet.h>
using namespace std;

#define WM_FTP_LOGTEXT  WM_USER+1002

/////////////////////////////////////////////////////////////////////////////
class CNetSess : public CInternetSession
{
public:
	CNetSess(LPCTSTR pstrAgent = NULL, DWORD dwContext = 1, DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG, LPCTSTR pstrProxyName = NULL, LPCTSTR pstrProxyBypass = NULL, DWORD dwFlags = 0 ) 
		: CInternetSession(pstrAgent, dwContext, dwAccessType, pstrProxyName, pstrProxyBypass, dwFlags) 
		{ 	
			m_pMainWnd = NULL;	
		};

// Attributes
public:
	HWND m_pMainWnd;  // pointer to parent window
// Operations
public:
	const TCHAR*  GetErrorString(DWORD dwErrorCode);
	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInfomration, DWORD dwStatusInformationLen);
};


class Ftp_Client  
{
public:
	Ftp_Client();
	virtual ~Ftp_Client();
    void    Close();
    BOOL    OpenSession(LPCTSTR appName, HWND hWnd, DWORD preconfig/*=PRE_CONFIG_INTERNET_ACCESS*/);
    BOOL    Connect(LPCTSTR strServerName, LPCTSTR szlogin, LPCTSTR password, UINT nport, BOOL pasivemode);
    void    LogOut(LPCTSTR psz,...);
    BOOL    SetCurrentDirectory(LPCTSTR strPath);
    BOOL    IsStillConnected();
    BOOL    CreateDirectory(LPCTSTR szDir);
    void    SetLogOutWindow(HWND);
    
    const   vector<WIN32_FIND_DATA>& ListLocation(LPCTSTR szDirectory=0);
    
    const TCHAR* GetLastResponceInfo();


    CFtpConnection              *p_FtpConnection;
    CNetSess                    *p_INETSESS;
    HWND                        m_hWndMain;
    string                      sz_lastdir;    
    vector<WIN32_FIND_DATA>     v_lastList;
    DWORD                       m_dwLastRespInfo;
};



#endif // !defined(AFX_FTP_CLIENT_H__06713E04_A9AD_4815_AEF6_62FB1462951B__INCLUDED_)
