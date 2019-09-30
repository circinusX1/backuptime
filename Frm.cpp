//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include "backupiime.h"
#include "Frm.h"
#include "BackUpiImeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CFrm, CFrameWnd)

CFrm::CFrm():m_trayIcon(IDI_TRAYICON)
{
    _lastIcon = 0;
    _lastTime = 0;
    _pModal   = 0;
}

CFrm::~CFrm()
{
}


BEGIN_MESSAGE_MAP(CFrm, CFrameWnd)
	//{{AFX_MSG_MAP(CFrm)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_COMMAND(MN_ENABLE, OnEnable)
	ON_COMMAND(MN_DISABLE, OnDisable)
	ON_COMMAND(MN_SETTINGS, OnSettings)
	ON_COMMAND(MN_EXIT, OnExit)
	ON_COMMAND(MN_STOP, OnStop)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTryNotify)
    ON_MESSAGE(WM_WORKING, OnWorking)
    ON_MESSAGE(WM_FTP_LOGTEXT, OnFTPMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrm message handlers

int CFrm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
    if(theApp._enabled)
    {
        m_trayIcon.SetIcon(IDI_ENABLED);
#ifdef _DEBUG
        _timer = SetTimer(3334, 10000,0); // 15 min interval
#else
        _timer = SetTimer(3334, 1000*5*60,0); // 5 min interval
#endif //
    }
    else
    {
        m_trayIcon.SetIcon(IDI_DISABLED);
    }
	return 0;
}

void CFrm::OnDestroy() 
{
	CFrameWnd::OnDestroy();
    if(_timer)
        KillTimer(_timer);
    _timer = 0;
}

LONG CFrm::OnFTPMessage(WPARAM wParam, LPARAM lParam) 
{
    return 0;
}

LRESULT CFrm::OnTryNotify(WPARAM w,LPARAM l)
{
    return m_trayIcon.OnTrayNotification(w, l);
}

LRESULT CFrm::OnWorking(WPARAM wParam, LPARAM l)
{
    if(wParam==1)
    {
        m_trayIcon.SetIcon(_lastIcon = IDI_BACKUPING2);
        theApp._working = 1;
    }
    else if(wParam==2)
    {
        DWORD dwc = GetTickCount();
        if(dwc - _lastTime > 500)
        {
            if(_lastIcon == IDI_BACKUPING2)
            {
                _lastIcon = IDI_BACKUPING1;
            }
            else
            {
                _lastIcon = IDI_BACKUPING2;
            }
            m_trayIcon.SetIcon(_lastIcon);
            _lastTime = dwc;
        }
        if(l && _pModal)
        {
            _pModal->GetDlgItem(EF_FNAME)->SetWindowText((LPCTSTR)l);
        }
    }
    else if(wParam==0)
    {
        _lastIcon       = 0;
        theApp._working = 0;
        if(theApp._enabled)
            m_trayIcon.SetIcon(IDI_ENABLED);
        else
            m_trayIcon.SetIcon(IDI_DISABLED);
        theApp.back_process.Break();
    }
    return 0;
}

void CFrm::OnTimer(UINT nIDEvent) 
{
    theApp.CheckTimer(0);	
	CFrameWnd::OnTimer(nIDEvent);
}

void CFrm::OnEnable() 
{
    theApp._enabled=1;
    if(_timer==0)
    {
        _timer = SetTimer(3334, 10000,0);
    }
    m_trayIcon.SetIcon(IDI_ENABLED);
}

void CFrm::OnDisable() 
{
    theApp._enabled=0;
    if(_timer)
    {
        KillTimer(_timer);
        _timer = 0;
    }
    m_trayIcon.SetIcon(IDI_DISABLED);
}

void CFrm::OnSettings() 
{
    if(theApp._settingsOn || theApp._working)
        return ;

    theApp._settingsOn = 1;
    theApp.back_process.Break(0);
    CBackUpiImeDlg d;
    _pModal = &d;
    d.DoModal();
    _pModal = 0;
    theApp.back_process.Break(1);
    theApp._settingsOn = 0;
}

void CFrm::OnExit() 
{
    if(_pModal)
    {
        ((CBackUpiImeDlg*)_pModal)->OnCancel();
    }
    PostQuitMessage(0);    
}

void CFrm::OnStop() 
{
    if(theApp._settingsOn)
        return ;
    theApp._settingsOn = 1;
    if(theApp._working)
        theApp.back_process.Break();
    theApp._settingsOn = 0;
}
