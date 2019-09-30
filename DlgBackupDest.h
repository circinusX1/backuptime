//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_DLGBACKUPDEST_H__8863DAC5_A8A9_45AC_A645_4CA9DC75A0E8__INCLUDED_)
#define AFX_DLGBACKUPDEST_H__8863DAC5_A8A9_45AC_A645_4CA9DC75A0E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBackupDest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupDest dialog

class CDlgBackupDest : public CDialog
{
// Construction
public:
	CDlgBackupDest(CWnd* pParent = NULL);   // standard constructor
	afx_msg void OnApply();
// Dialog Data
	//{{AFX_DATA(CDlgBackupDest)
	enum { IDD = DLG_BACKUPDETINATION };
	BOOL	m_logfiles;
	BOOL	m_overwritefiles;
	BOOL	m_treestruct;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBackupDest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBackupDest)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnNetexplore();
	afx_msg void OnFtpSettings();
	afx_msg void OnFtpSettings2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    BOOL    _bFTPOK;
    TCHAR   m_lastfolder[_MAX_PATH];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBACKUPDEST_H__8863DAC5_A8A9_45AC_A645_4CA9DC75A0E8__INCLUDED_)
