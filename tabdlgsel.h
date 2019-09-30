//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005

#if !defined(AFX_TABDLGSEL_H__0224E6CD_BABC_4F30_87C5_1A00A9025FAC__INCLUDED_)
#define AFX_TABDLGSEL_H__0224E6CD_BABC_4F30_87C5_1A00A9025FAC__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// TabDlgSel window
class TabDlgSel : public CTabCtrl
{
// Construction
public:
	TabDlgSel();

// Attributes
public:

// Operations
public:
	void AddDialog(CDialog * pDlg, LPCSTR pTitle,int iMage);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TabDlgSel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TabDlgSel();

	// Generated message map functions
public:
	//{{AFX_MSG(TabDlgSel)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
    CDialog* _pParent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDLGSEL_H__0224E6CD_BABC_4F30_87C5_1A00A9025FAC__INCLUDED_)
