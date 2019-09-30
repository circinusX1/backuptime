//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_BACKUPIIMEDLG_H__F23C21AC_151C_45E7_A7D6_0E6A7D81674A__INCLUDED_)
#define AFX_BACKUPIIMEDLG_H__F23C21AC_151C_45E7_A7D6_0E6A7D81674A__INCLUDED_

#include "tabdlgsel.h"
#include "DlgPickFolders.h"
#include "DlgBackupDest.h"
#include "DlgSchedule.h"
/////////////////////////////////////////////////////////////////////////////
// CBackUpiImeDlg dialog
#define WM_PB_APPLY (WM_USER+1000) //afx_msg LRESULT OnMsgApply(WPARAM,LPARAM){OnApply(); return 0;};

class CBackUpiImeDlg : public CDialog
{
// Construction
public:
	CBackUpiImeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBackUpiImeDlg)
	enum { IDD = IDD_BACKUPIIME_DIALOG };
	TabDlgSel	m_tab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackUpiImeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBackUpiImeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
public:
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnApply();
    afx_msg LRESULT OnMsgApply(WPARAM,LPARAM){OnApply(); return 0;};

	//}}AFX_MSG
    LRESULT OnLog(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

    CDlgPickFolders   dlg_pickfolders;
    CDlgBackupDest    dlg_dest;   
    CDlgSchedule      dlg_schedule;  
    afx_msg void OnStnClickedFname();
};


extern CEdit* HLog;//extern CEdit* HLog;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKUPIIMEDLG_H__F23C21AC_151C_45E7_A7D6_0E6A7D81674A__INCLUDED_)
