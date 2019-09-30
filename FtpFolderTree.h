//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_FTPFOLDERTREE_H__51502DB9_AA2C_4508_825E_3D5724F9DD1B__INCLUDED_)
#define AFX_FTPFOLDERTREE_H__51502DB9_AA2C_4508_825E_3D5724F9DD1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FtpFolderTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFtpFolderTree window

class CFtpFolderTree : public CTreeCtrl
{
// Construction
public:
	CFtpFolderTree();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpFolderTree)
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CFtpFolderTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFtpFolderTree)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPFOLDERTREE_H__51502DB9_AA2C_4508_825E_3D5724F9DD1B__INCLUDED_)
