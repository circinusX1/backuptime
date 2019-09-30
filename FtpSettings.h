//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#if !defined(AFX_FTPSETTINGS_H__C026118A_E834_4B1A_B09C_4C8E8653C314__INCLUDED_)
#define AFX_FTPSETTINGS_H__C026118A_E834_4B1A_B09C_4C8E8653C314__INCLUDED_



#include "FolderTree.h"
#include "Ftp_Client.h"
#include "GListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CFtpSettings dialog

class CFtpSettings : public CDialog
{
// Construction
public:
	CFtpSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFtpSettings)
	enum { IDD = IDD_DIALOG1 };
	CGListCtrl	m_lbfiles;
	CFolderTree	m_tree;
	CEdit	m_out;
	CString	m_host;
	UINT	m_port;
	CString	m_login;
	CString	m_password;
	CString	m_path;
	BOOL	m_pasivemode;
	BOOL	m_wrc;
	//}}AFX_DATA
    BOOL    m_idFTP;
    CString m_selpath;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFtpSettings)
	virtual void OnCancel();
	afx_msg void OnConnect();
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimecheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    void LogOut(LPCTSTR psz) ;
    void  PopulateTree();
    BOOL IsStillConnected();
    BOOL ExploreDirectory(const CString& strDir, HTREEITEM hParent, BOOL bRefresh=0);
    void TestFileCopy();
public:
    BOOL                _success;
    Ftp_Client          ftp_client;
    HTREEITEM           m_hCurrentTreeItem;
    BOOL                b_frompopulate;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPSETTINGS_H__C026118A_E834_4B1A_B09C_4C8E8653C314__INCLUDED_)
