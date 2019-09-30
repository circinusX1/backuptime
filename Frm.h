//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_FRM_H__B9CAD43D_5DAB_4C34_8F44_2C51A60E46A7__INCLUDED_)
#define AFX_FRM_H__B9CAD43D_5DAB_4C34_8F44_2C51A60E46A7__INCLUDED_

#include "TrayIcon.h"



class CFrm : public CFrameWnd
{
	DECLARE_DYNCREATE(CFrm)
    CFrm();           // protected constructor used by dynamic creation
protected:


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrm)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrm();

	// Generated message map functions
	//{{AFX_MSG(CFrm)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnable();
	afx_msg void OnDisable();
	afx_msg void OnSettings();
	afx_msg void OnExit();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    LRESULT OnTryNotify(WPARAM,LPARAM);
    LRESULT OnFTPMessage (WPARAM,LPARAM);
    LONG    OnWorking(WPARAM wParam, LPARAM lParam) ;
    UINT    _timer;  

    CTrayIcon   m_trayIcon;
    UINT        _lastIcon;
    DWORD       _lastTime;
    CDialog*    _pModal;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRM_H__B9CAD43D_5DAB_4C34_8F44_2C51A60E46A7__INCLUDED_)
