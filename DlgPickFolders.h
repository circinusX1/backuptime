//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_DLGPICKFOLDERS_H__605F30A7_E70C_4117_9C58_5C531B439052__INCLUDED_)
#define AFX_DLGPICKFOLDERS_H__605F30A7_E70C_4117_9C58_5C531B439052__INCLUDED_

#include "GListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPickFolders dialog

class CDlgPickFolders : public CDialog
{
// Construction
public:
	CDlgPickFolders(CWnd* pParent = NULL);   // standard constructor

    void OnApply();
// Dialog Data
	//{{AFX_DATA(CDlgPickFolders)
	enum { IDD = DLG_PICKFOLDERS };
	CGListCtrl	m_folderlist;
	//}}AFX_DATA

    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPickFolders)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPickFolders)
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnAddfolder();
	afx_msg void OnEditfolder();
	afx_msg void OnDeleteFolder();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    TCHAR   m_lastfolder[_MAX_PATH];
    CString m_lastfilters;
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPICKFOLDERS_H__605F30A7_E70C_4117_9C58_5C531B439052__INCLUDED_)
