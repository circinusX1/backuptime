//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_DLGSCHEDULE_H__5CDA73F8_56B9_4604_AE1E_F3308BC19414__INCLUDED_)
#define AFX_DLGSCHEDULE_H__5CDA73F8_56B9_4604_AE1E_F3308BC19414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSchedule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSchedule dialog

class CDlgSchedule : public CDialog
{
// Construction
public:
	CDlgSchedule(CWnd* pParent = NULL);   // standard constructor
	afx_msg void OnApply();
// Dialog Data
	//{{AFX_DATA(CDlgSchedule)
	enum { IDD = DLG_SCHEDULE };
	CEdit	m_out;
	CComboBox	m_cbtime;
	CComboBox	m_cbday;
	CComboBox	m_cbmonth;
	BOOL	m_backhook;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSchedule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSchedule)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBackupnow();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnHookchange();
	//}}AFX_MSG
    LRESULT OnFTPMessage(WPARAM wParam, LPARAM l);
	DECLARE_MESSAGE_MAP()

    BOOL    _bckuping;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCHEDULE_H__5CDA73F8_56B9_4604_AE1E_F3308BC19414__INCLUDED_)
