//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include "backupiime.h"
#include "DlgSchedule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSchedule dialog


CDlgSchedule::CDlgSchedule(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSchedule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSchedule)
	m_backhook = FALSE;
	//}}AFX_DATA_INIT
    _bckuping = 0;
}


void CDlgSchedule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSchedule)
	DDX_Control(pDX, IDC_EDIT1, m_out);
	DDX_Control(pDX, CB_TIME, m_cbtime);
	DDX_Control(pDX, CB_DAY, m_cbday);
	DDX_Control(pDX, CB_MONTH, m_cbmonth);
	DDX_Check(pDX, CK_HOOKCHANGE, m_backhook);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CDlgSchedule, CDialog)
	//{{AFX_MSG_MAP(CDlgSchedule)
	ON_WM_DESTROY()
	ON_BN_CLICKED(PB_BACKUPNOW, OnBackupnow)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(CK_HOOKCHANGE, OnHookchange)
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_FTP_LOGTEXT, OnFTPMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSchedule message handlers

BOOL CDlgSchedule::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_cbmonth.SetCurSel(theApp._month2Run+1);
    m_cbtime.SetCurSel(theApp._hour2Run+1);
    m_cbday.SetCurSel(theApp._dayOfWeek + 1);
    
    CheckDlgButton(CK_SHOWERRORMSGBOX, theApp.b_showerrormsgbox ? BST_CHECKED:BST_UNCHECKED);
    CheckDlgButton(CK_WORKINBACKGROUND, theApp.b_workinbackground? BST_CHECKED:BST_UNCHECKED);
    CheckDlgButton(CK_TASKBAR, theApp.b_taskbar? BST_CHECKED:BST_UNCHECKED);
    CheckDlgButton(CK_LOGERRORS, theApp.b_logerrors? BST_CHECKED:BST_UNCHECKED);
    CheckDlgButton(CK_CHANGEFT, theApp.b_changeft? BST_CHECKED:BST_UNCHECKED);
    
	
	return TRUE;  
}

void CDlgSchedule::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CDlgSchedule::OnBackupnow() 
{
    theApp.SaveSettings();
    CWaitCursor cwc;
    if(_bckuping)
    {
        theApp.back_process.Break();
        _bckuping = 0;
    }
    else
    {
        m_out.SetWindowText("");
        SetDlgItemText(PB_BACKUPNOW, "Break &Backup");
        _bckuping = 1;
        BOOL oo = theApp._allwaysOverwrite;
        theApp._allwaysOverwrite = 1;
	    theApp.CheckTimer((UINT)m_hWnd);

        while(theApp._working)
        {
            do{
                CWaitCursor cwc;
                theApp.PumpMessage();
                Sleep(4); //priority
            }while(0);
            Sleep(100); //priority
            
        }
        Sleep(512);
        SetDlgItemText(PB_BACKUPNOW, "&Test Backup Now");
        Beep(600,50);
        theApp._allwaysOverwrite = oo;
        m_out.SetSel(-1,-1);

        if(theApp.back_process._nTotalFiles!=0)
        {
            if(theApp.back_process._nCopyedFiles == theApp.back_process._nTotalFiles)
            {
                m_out.ReplaceSel("All Files Were copyed to destination Location\r\n");            
            }
            else
            {
                 m_out.ReplaceSel(MKSTR("Copyed %d Files out of %d \r\n",theApp.back_process._nCopyedFiles,
                                                                              theApp.back_process._nTotalFiles));            
            }
        }
        else
        {
            m_out.ReplaceSel("No Changes or ftp errors\r\n");            
        }

        m_out.ReplaceSel("End Of Report.\r\n");
        _bckuping = 0;
    }
}

void CDlgSchedule::OnOk() 
{

}

void CDlgSchedule::OnCancel() 
{
}

void CDlgSchedule::OnApply() 
{
    theApp._month2Run         =m_cbmonth.GetCurSel()-1;
    theApp._hour2Run          =m_cbtime.GetCurSel()-1;
    theApp._dayOfWeek         =m_cbday.GetCurSel()-1;


    theApp.b_showerrormsgbox  = IsDlgButtonChecked(CK_SHOWERRORMSGBOX);
    theApp.b_workinbackground = IsDlgButtonChecked(CK_WORKINBACKGROUND);
    theApp.b_taskbar          = IsDlgButtonChecked(CK_TASKBAR);
    theApp.b_logerrors        = IsDlgButtonChecked(CK_LOGERRORS);
    theApp.b_changeft          = IsDlgButtonChecked(CK_CHANGEFT);
    theApp._priority          = 1;

}

LRESULT CDlgSchedule::OnFTPMessage(WPARAM, LPARAM l)
{
    if(l)
    {
        TCHAR* psz = (TCHAR*)l;
        if(psz && psz[0]=='+') // leave warnings
        {
            m_out.SetSel(-1,-1);
            m_out.ReplaceSel(psz);
            
            if(psz[strlen(psz)-1]!='\n')
            {
                m_out.SetSel(-1,-1);
                m_out.ReplaceSel("\r\n");
            }
        }
    }
    return 0;
}

void CDlgSchedule::OnHookchange() 
{
    //m_backhook	
}
