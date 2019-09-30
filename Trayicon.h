//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#ifndef _TRAYICON_H
#define _TRAYICON_H

#define WM_MY_TRAY_NOTIFICATION WM_USER+2005

//-----------------------------------------------------------------------------
class CTrayIcon : public CCmdTarget {
protected:
	DECLARE_DYNAMIC(CTrayIcon)
	NOTIFYICONDATA m_nid;			// struct for Shell_NotifyIcon args

public:
	CTrayIcon(UINT uID);
	~CTrayIcon();
	// Call this to receive tray notifications
	void SetNotificationWnd(CWnd* pNotifyWnd, UINT uCbMsg);
	BOOL SetIcon(UINT uID); 
	BOOL SetIcon(HICON hicon, LPCSTR lpTip);
	BOOL SetIcon(LPCTSTR lpResName, LPCSTR lpTip)
		{ return SetIcon(lpResName ? 
			AfxGetApp()->LoadIcon(lpResName) : NULL, lpTip); }
	BOOL SetStandardIcon(LPCTSTR lpszIconName, LPCSTR lpTip)
		{ return SetIcon(::LoadIcon(NULL, lpszIconName), lpTip); }
	virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
};

#endif
