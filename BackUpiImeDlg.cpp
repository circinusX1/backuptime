//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include "BackUpiIme.h"
#include "BackUpiImeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEdit* HLog = 0;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackUpiImeDlg dialog

CBackUpiImeDlg::CBackUpiImeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackUpiImeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBackUpiImeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBackUpiImeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackUpiImeDlg)
	DDX_Control(pDX, TAB_CTRL, m_tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBackUpiImeDlg, CDialog)
	//{{AFX_MSG_MAP(CBackUpiImeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(PB_APPLY, OnApply)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_FTP_LOGTEXT, OnLog)
    ON_MESSAGE(WM_PB_APPLY, OnMsgApply)
    ON_STN_CLICKED(EF_FNAME, &CBackUpiImeDlg::OnStnClickedFname)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackUpiImeDlg message handlers

BOOL CBackUpiImeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
    m_tab._pParent=this;

#ifdef _DEBUG
    theApp._adminMode = 1;
#endif //

    if(theApp._adminMode)
    {
	    dlg_pickfolders.Create(DLG_PICKFOLDERS, this);
        m_tab.AddDialog(&dlg_pickfolders, "Select Folders/Files for Backup", 0);

        dlg_dest.Create(DLG_BACKUPDETINATION, this);
        m_tab.AddDialog(&dlg_dest, "Select FTP/Remote Backup Location", 0);
    }

    dlg_schedule.Create(DLG_SCHEDULE, this);
    m_tab.AddDialog(&dlg_schedule, "Backup Settings", 0);


    LRESULT lr;
    m_tab.SetCurSel(0);
    m_tab.OnSelchange(0, &lr) ;
    //dlg_pickfolders.ShowWindow(SW_SHOW);

    CString csTitle; GetWindowText(csTitle);
    if(theApp._adminMode)
    {
        csTitle+= " Administrator Mode";
    }
    else
    {
        csTitle+= " User Mode ";
    }
    SetWindowText(csTitle);

    


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBackUpiImeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBackUpiImeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBackUpiImeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBackUpiImeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CBackUpiImeDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}


void CBackUpiImeDlg::OnApply() 
{
    if(theApp._adminMode)
    {
        dlg_pickfolders.OnApply();
        dlg_dest.OnApply();
    }
    dlg_schedule.OnApply();
    theApp.back_process.Clear();
    theApp.SaveSettings();
}

LRESULT CBackUpiImeDlg::OnLog(WPARAM, LPARAM sz)
{
    if(HLog && ::IsWindow(HLog->m_hWnd))
    {
        HLog->SetSel(-1,-1);
        HLog->ReplaceSel((LPCTSTR)sz);
    }
    else
        HLog=0;
    return 0;
}

void CBackUpiImeDlg::OnStnClickedFname()
{
    // TODO: Add your control notification handler code here
}
