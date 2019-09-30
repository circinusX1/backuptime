//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include "resource.h"
#include "backupiime.h"
#include "DlgBackupDest.h"
#include "FtpSettings.h"
#include "DlgFolderSelect.h"
#include "Ftp_Client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupDest dialog


CDlgBackupDest::CDlgBackupDest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBackupDest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBackupDest)
	m_logfiles = FALSE;
	m_overwritefiles = FALSE;
	m_treestruct = FALSE;
	//}}AFX_DATA_INIT
    _bFTPOK = 0;
}


void CDlgBackupDest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBackupDest)
	DDX_Check(pDX, CK_REPORT, m_logfiles);
	DDX_Check(pDX, CK_OVERWRITE, m_overwritefiles);
	DDX_Check(pDX, CK_CREATETREE, m_treestruct);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBackupDest, CDialog)
	//{{AFX_MSG_MAP(CDlgBackupDest)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(PB_NETEXPLORE, OnNetexplore)
	ON_BN_CLICKED(PB_FTP_SETTINGS, OnFtpSettings)
	ON_BN_CLICKED(PB_FTP_SETTINGS2, OnFtpSettings2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBackupDest message handlers

void CDlgBackupDest::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CDlgBackupDest::OnInitDialog() 
{
	CDialog::OnInitDialog();
    CheckRadioButton(IDC_RADIO1, IDC_RADIO2, theApp._isFtp ? IDC_RADIO2 : IDC_RADIO1);


    CheckDlgButton(CK_REPORT,   theApp._logfiles?BST_CHECKED:BST_UNCHECKED);
    CheckDlgButton(CK_OVERWRITE,theApp._allwaysOverwrite?BST_CHECKED:BST_UNCHECKED);
    SetDlgItemText(ST_LOCATION, theApp._backupLocation);
    SetDlgItemText(ST_SERVER,  theApp._ftpIP);
    SetDlgItemInt(ST_PORT, theApp._ftpPort);
    SetDlgItemText(ST_PASSWORD,"*********");
    SetDlgItemText(ST_USER, theApp._ftpUser);

    SetDlgItemText(EF_DETFOLDER, theApp._remoteFolder);

    m_logfiles=theApp._logfiles ;
    m_overwritefiles=theApp._allwaysOverwrite  ;
    m_treestruct    = theApp.m_treestruct;

    
    UpdateData(0);

    if(IsDlgButtonChecked(IDC_RADIO1))
        OnRadio1();
    else
        OnRadio2();

	return TRUE;  
	              
}

void CDlgBackupDest::OnCancel() 
{
}

void CDlgBackupDest::OnOk() 
{
	
}

void CDlgBackupDest::OnApply() 
{
    UpdateData(1);
    theApp._logfiles = m_logfiles;
    theApp._allwaysOverwrite = m_overwritefiles ;
    theApp.m_treestruct = m_treestruct;

    theApp._isFtp = 0;

    if(IsDlgButtonChecked(IDC_RADIO2))
    {
        theApp._isFtp = 1;

        CWaitCursor cws;
        Ftp_Client fc;
        if(fc.OpenSession("testSess", 0, PRE_CONFIG_INTERNET_ACCESS))
        {
            if(fc.Connect(theApp._ftpIP, theApp._ftpUser, theApp._ftpPasssword, theApp._ftpPort, theApp._passsiveMode))
            {
                fc.Close();
                theApp._isFtp = 1;
                return;
            }
        }
        fc.Close();
        theApp._isFtp =0;
     
        AfxMessageBox("Cannot Connect to FTP Server. Please Chek FTP settings", MB_OK|MB_ICONINFORMATION);
    }
    theApp.SaveSettings();
}

void CDlgBackupDest::OnRadio1() 
{

    GetDlgItem(EF_DETFOLDER)->EnableWindow(1);
    GetDlgItem(PB_NETEXPLORE)->EnableWindow(1);
    GetDlgItem(PB_FTP_SETTINGS2)->EnableWindow(1);

    GetDlgItem(ST1)->EnableWindow(0);
    GetDlgItem(ST2)->EnableWindow(0);
    GetDlgItem(ST3)->EnableWindow(0);
    GetDlgItem(ST4)->EnableWindow(0);
    GetDlgItem(ST5)->EnableWindow(0);
    GetDlgItem(ST_LOCATION)->EnableWindow(0);
    GetDlgItem(ST_SERVER)->EnableWindow(0);
    GetDlgItem(ST_PORT)->EnableWindow(0);
    GetDlgItem(ST_PASSWORD)->EnableWindow(0);
    GetDlgItem(ST_USER)->EnableWindow(0);

    GetDlgItem(PB_FTP_SETTINGS)->EnableWindow(0);

}

void CDlgBackupDest::OnRadio2() 
{
    GetDlgItem(ST1)->EnableWindow(1);
    GetDlgItem(ST2)->EnableWindow(1);
    GetDlgItem(ST3)->EnableWindow(1);
    GetDlgItem(ST4)->EnableWindow(1);
    GetDlgItem(ST5)->EnableWindow(1);
    GetDlgItem(ST_LOCATION)->EnableWindow(1);
    GetDlgItem(ST_SERVER)->EnableWindow(1);
    GetDlgItem(ST_PORT)->EnableWindow(1);
    GetDlgItem(ST_PASSWORD)->EnableWindow(1);
    GetDlgItem(ST_USER)->EnableWindow(1);

    
    GetDlgItem(PB_FTP_SETTINGS)->EnableWindow(1);

    GetDlgItem(EF_DETFOLDER)->EnableWindow(0);
    GetDlgItem(PB_NETEXPLORE)->EnableWindow(0);
    GetDlgItem(PB_FTP_SETTINGS2)->EnableWindow(0);
    

}

void CDlgBackupDest::OnNetexplore() 
{
    CDlgFolderSelect d;
    d.b_localdrives = 0;
    d.b_mappeddrives= 1;
    d.b_hideFileTypes = 1;
    d.m_sPath         = theApp._remoteFolder;
    d.m_Title        = "Select Backup Folder ";

    if(IDOK == d.DoModal())
    {
        SetDlgItemText(EF_DETFOLDER, d.m_sPath);
        theApp._remoteFolder = d.m_sPath;
    }
}

void CDlgBackupDest::OnFtpSettings() 
{
    CFtpSettings    d;

    
    if(IDOK == d.DoModal())
    {
        _bFTPOK = d._success;
        if(d._success)
        {
            SetDlgItemText(ST_LOCATION,d.m_path);
            SetDlgItemText(ST_SERVER,d.m_host);
            SetDlgItemInt(ST_PORT,d.m_port);
            SetDlgItemText(ST_PASSWORD,"********");
            SetDlgItemText(ST_USER,d.m_login);
            _bFTPOK = 1;
        }
    }
}

void CDlgBackupDest::OnFtpSettings2() 
{
	
}
