//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_FOLDERTREE_H__93455F9D_245C_4044_B1D3_E70A1CED2BE7__INCLUDED_)
#define AFX_FOLDERTREE_H__93455F9D_245C_4044_B1D3_E70A1CED2BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderTree window

class CFolderTree : public CTreeCtrl
{
// Construction
public:
	CFolderTree();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderTree)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFolderTree();
    HTREEITEM FindItem(LPCTSTR lpszText, HTREEITEM hItem);
    HTREEITEM GetNextItemEx(HTREEITEM hItem);

	// Generated message map functions
protected:
	//{{AFX_MSG(CFolderTree)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERTREE_H__93455F9D_245C_4044_B1D3_E70A1CED2BE7__INCLUDED_)
