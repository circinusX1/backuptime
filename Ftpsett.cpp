// FtpSett.cpp : implementation file
//

#include "stdafx.h"
#include "backupiime.h"
#include "FtpSett.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFtpSett dialog


CFtpSett::CFtpSett(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpSett::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtpSett)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFtpSett::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpSett)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFtpSett, CDialog)
	//{{AFX_MSG_MAP(CFtpSett)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpSett message handlers
