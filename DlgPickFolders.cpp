//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include "backupiime.h"
#include "DlgPickFolders.h"
#include "DlgPickFolders.h"
#include "DlgFolderSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPickFolders dialog


CDlgPickFolders::CDlgPickFolders(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPickFolders::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPickFolders)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    ::_tcscpy(m_lastfolder , "C:\\");
    m_lastfilters="*.*";

}


void CDlgPickFolders::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPickFolders)
	DDX_Control(pDX, IDC_LIST1, m_folderlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPickFolders, CDialog)
	//{{AFX_MSG_MAP(CDlgPickFolders)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(PB_ADDFOLDER, OnAddfolder)
	ON_BN_CLICKED(PB_EDITFOLDER, OnEditfolder)
	ON_BN_CLICKED(PB_DELETE_FOLDER, OnDeleteFolder)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPickFolders message handlers

void CDlgPickFolders::OnCancel() 
{
}

void CDlgPickFolders::OnAddfolder() 
{
    CDlgFolderSelect d;
    d.b_localdrives = 1;
    d.b_mappeddrives= 0;
    d.m_fileFilters = m_lastfilters;
    d.m_sPath       = m_lastfolder;
    d.m_Title       = "Select Folder and File Filters";
    if(IDOK == d.DoModal())
    {
        m_lastfilters = d.m_fileFilters;
        ::_tcscpy(m_lastfolder  , d.m_sPath);
        int isel = m_folderlist.AddNewItem(m_lastfolder, m_lastfilters, 0);
        m_folderlist.SetCurSel(isel);

        GetDlgItem(PB_DELETE_FOLDER)->EnableWindow(m_folderlist.GetItemCount()!=0);
    }
	
}


void CDlgPickFolders::OnEditfolder() 
{
    CDlgFolderSelect d;

    int isel = m_folderlist.GetCurSel();
    if(isel==-1)
        return;

    d.m_sPath       = m_folderlist.GetItemText(isel, 0);
    d.m_fileFilters = m_folderlist.GetItemText(isel, 1);
    d.m_Title       = "Modify Folder or File Filters";
    if(IDOK == d.DoModal())
    {
        m_folderlist.LstSetItem(isel, d.m_sPath, d.m_fileFilters, 0);
        m_folderlist.SetCurSel(isel);
    }
    GetDlgItem(PB_DELETE_FOLDER)->EnableWindow(m_folderlist.GetItemCount()!=0);
}

void CDlgPickFolders::OnDeleteFolder() 
{
    int isel = m_folderlist.GetCurSel();
    if(isel==-1)
        return;

    CString  szpath = m_folderlist.GetItemText(isel, 0);
    CString  csf;
    csf.Format("Do You want to delete '%s' From Backup List", szpath);

    UINT nok = AfxMessageBox(csf, MB_YESNO|MB_ICONQUESTION);
    if(nok == IDNO)
        return;
	
    if(isel>=0)
    {
        m_folderlist.DeleteItem(isel);
    }
    UINT ic = m_folderlist.GetItemCount();
    if(ic != 0)
    {
        --isel;
        if(isel<0)
            isel=0;

        m_folderlist.SetCurSel(isel);
    }
    GetDlgItem(PB_DELETE_FOLDER)->EnableWindow(ic!=0);	

}

BOOL CDlgPickFolders::OnInitDialog() 
{
	CDialog::OnInitDialog();
    m_folderlist.SetHeadersNames("Folder                                ", "Files      ", 0);

    vector<BackupFolders>::iterator b = theApp._vFolders.begin();
    vector<BackupFolders>::iterator e = theApp._vFolders.end();
    for(; b!=e; b++)
    {
        m_folderlist.AddNewItem(b->folder.c_str(), b->filter.c_str(), 0);
    }

    

    GetDlgItem(PB_DELETE_FOLDER)->EnableWindow(m_folderlist.GetItemCount()!=0);	
	return TRUE;  
}

void CDlgPickFolders::OnDestroy() 
{
	CDialog::OnDestroy();
}
void CDlgPickFolders::OnOk(){} 


void CDlgPickFolders::OnApply()
{
    UINT count = m_folderlist.GetItemCount();
    CString szpath ;
    CString szfiles ;
    CString entry ;
    BackupFolders cbf;

    theApp._vFolders.clear();
    
    theApp.WriteProfileInt("FOLDERS","COUNT",count);
    for(int i=0;i<count;i++)
    {
        szpath = m_folderlist.GetItemText(i, 0);
        szfiles = m_folderlist.GetItemText(i, 1);

        cbf.folder = (LPCTSTR)szpath;
        cbf.filter = (LPCTSTR)szfiles;
        theApp._vFolders.push_back(cbf);
    }
}


void CDlgPickFolders::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
    m_folderlist.Invalidate();
    	
}
