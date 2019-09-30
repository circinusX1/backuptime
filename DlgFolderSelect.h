//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_DLGFOLDERSELECT_H__2D630FA8_F7C7_4B78_87E7_429208E85FAF__INCLUDED_)
#define AFX_DLGFOLDERSELECT_H__2D630FA8_F7C7_4B78_87E7_429208E85FAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFolderSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFolderSelect dialog

class CDlgFolderSelect : public CDialog
{
// Construction
public:
	CDlgFolderSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFolderSelect)
	enum { IDD = DLG_SELECTFOLDER };
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	//Quick and Nasty one liners.
	CString GetPath() const					{ return m_sPath; }
	void	SetPath( CString sPath )		{ m_sPath = sPath; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFolderSelect)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFolderSelect)
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOk();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//Operators
private:
	CString GetItemPath( HTREEITEM hItem );
	bool	PopulateTree( CString sPath, HTREEITEM hParent );
	HTREEITEM InsertItem( HTREEITEM hParent, NETRESOURCE *const pNetResource, CString sText, int iImage, int iImageSelected = -1 );
	bool	EnumNetwork( HTREEITEM hParent );

//Attributes
private:
	CImageList	m_ImageListTree;					//Item in the Tree image list				
	HTREEITEM	m_hNetworkRoot;						//Network neighbourhood root

public:
	CString		m_sPath;							//Highlighted path
    CString     m_fileFilters;
    CString     m_Title;

    BOOL    b_localdrives;
    BOOL    b_mappeddrives;
    BOOL    b_hideFileTypes;

};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFOLDERSELECT_H__2D630FA8_F7C7_4B78_87E7_429208E85FAF__INCLUDED_)
