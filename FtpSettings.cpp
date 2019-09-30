//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include "backupiime.h"
#include "FtpSettings.h"
#include <Afxinet.h>
#include "BackUpiImeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
int GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected=0)
{
	static int _nDirIconIdx = -1;
	SHFILEINFO sfi;
	memset(&sfi, 0, sizeof(sfi));

	if (bIsDir)
	{
		SHGetFileInfo(lpszPath, 
			FILE_ATTRIBUTE_DIRECTORY, 
			&sfi, 
			sizeof(sfi), 
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES | (bSelected ? SHGFI_OPENICON : 0)
		);
		// Cache the index for the directory icon
		_nDirIconIdx = sfi.iIcon;
		return _nDirIconIdx;
	}
	else
	{
		SHGetFileInfo (lpszPath, 
			FILE_ATTRIBUTE_NORMAL, 
			&sfi, 
			sizeof(sfi), 
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES | (bSelected ? SHGFI_OPENICON : 0)
		);
		return sfi.iIcon;
	}
	return -1;
}




CFtpSettings::CFtpSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtpSettings)
	m_host = _T("");
	m_port = 0;
	m_login = _T("");
	m_password = _T("");
	m_path = _T("");
	m_pasivemode = FALSE;
	//}}AFX_DATA_INIT
    _success = 0;
    m_hCurrentTreeItem = 0;
    b_frompopulate = 0;
}


void CFtpSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpSettings)
	DDX_Control(pDX, LS_FILES, m_lbfiles);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, EF_OUT, m_out);
	DDX_Text(pDX, EF_HOST, m_host);
	DDX_Text(pDX, EF_PORT, m_port);
	DDX_Text(pDX, EF_LOGIN, m_login);
	DDX_Text(pDX, EF_PASSWORD, m_password);
	DDX_Text(pDX, EF_INITIALPATH, m_path);
	DDX_Check(pDX, IDC_CHECK1, m_pasivemode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFtpSettings, CDialog)
	//{{AFX_MSG_MAP(CFtpSettings)
	ON_BN_CLICKED(PB_CONNECT, OnConnect)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_BN_CLICKED(CK_TIMECHECK, OnTimecheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpSettings message handlers

void CFtpSettings::OnCancel() 
{
	CDialog::OnCancel();
}

void CFtpSettings::OnConnect() 
{
    _success = 0;

    GetDlgItem(IDOK)->EnableWindow(0);
    CWaitCursor cursor; 
    UpdateData(1);

    m_lbfiles.SetHeadersNames("File               ", "Time          ", "Size   ",0);

    ftp_client.Close();

    if(m_login.GetLength()==0 ||
        m_password.GetLength()==0 ||
        m_host.GetLength()==0 ||
        m_port ==0)
    {
        AfxMessageBox("One of the Log On Parameters is missing", MB_OK|MB_ICONINFORMATION);
        return;
    }

	CString str;
	if (!str.LoadString(AFX_IDS_APP_TITLE))
		str = _T("AppUnknown");

    LogOut(MKSTR("Connecting to: %s.. Please Wait \r\n", m_host));
    if(!ftp_client.OpenSession(str, m_hWnd, PRE_CONFIG_INTERNET_ACCESS))
    {
        AfxMessageBox(MKSTR("Cannot Open Intenet Connection: Error %d", GetLastError()), MB_OK|MB_ICONHAND);
        return;
    }

    if(!ftp_client.Connect(m_host, m_login, m_password, m_port, m_pasivemode))
    {
        return;
    }
    b_frompopulate=1;
    PopulateTree();
    b_frompopulate=0;
    if(IsDlgButtonChecked(CK_TIMECHECK))
        TestFileCopy();
    else
        _success = 1;
    GetDlgItem(IDOK)->EnableWindow(1);
    
}


void CFtpSettings::TestFileCopy()
{
    try{
        theApp.Redir();
        LogOut("Testing Ftp Remote Directory for Access Rights...\r\n");
        DWORD       d = 0;
        FILETIME    wtRemote={0};

        // create a test file to try to put it on the server:
        FILE* pf = ::fopen("~testfile.txt","wb");
        if(pf)
        {
            ::fputs("this is a backup watch test file\r\n",pf);
            ::fclose(pf);
            TCHAR cd[260];
            TCHAR f2c[260];
            _getcwd(cd,260);

            ::sprintf(f2c,"%s\\~testfile.txt",cd);
        
            int         repeat = 3;
        
            while(--repeat>0)
            {
                d = 0;
                if(ftp_client.p_FtpConnection->PutFile(f2c, "~testfile.txt"))
                {
                    Sleep(2000);
                
            	    CFtpFileFind ftpFind(ftp_client.p_FtpConnection);
                    if(ftpFind.FindFile("~testfile.txt"))
                    {
                        try{
                            ftpFind.FindNextFile();
                            ftpFind.GetLastWriteTime(&wtRemote);
                        }catch(...){}
                    }
                    ftpFind.Close();
                    break;
                }
                d = GetLastError();
                if(d == 12003)
                {
                    Sleep(256);
                }
                else
                {
                    break;
                }
            }
        }
        if(d)
        {
            LogOut(MKSTR("Error To Copy the File in Specified Location: Error %d. User have no write access\r\n", d));
            _success = 0;
            return;
        }
    
        if(wtRemote.dwLowDateTime || wtRemote.dwHighDateTime)
        {
	        WIN32_FIND_DATA	fDATA;	
	        HANDLE hfFind=FindFirstFile("~testfile.txt", &fDATA) ;
    
            if(hfFind != INVALID_HANDLE_VALUE)
            {
                FindClose(hfFind);  

                SYSTEMTIME rt;
                SYSTEMTIME lt;

                FileTimeToSystemTime(&wtRemote, &rt);
                FileTimeToSystemTime(&fDATA.ftLastWriteTime, &lt);

                if(rt.wMinute    != lt.wMinute ||
                       rt.wHour  != lt.wHour   ||
                       rt.wDay   != lt.wDay    ||
                       rt.wYear  != lt.wYear)
                {
                    LogOut("Remote Time Differs by the Local Sistem Time \r\n");
                    LogOut("This may result in wrong backuped file times\r\n");
                }
            }
        }
        ftp_client.p_FtpConnection->Remove("~testfile.txt");

        _success = 1;
    }catch(...)
    {
        LogOut("Unespected Error\r\n");
        _success = 0;
    }

}


void CFtpSettings::OnOk() 
{
    UpdateData(1);
    if(_success)
    {
        theApp._ftpIP           = m_host     ;
        theApp._ftpPort         = m_port     ;
        theApp._ftpUser         = m_login    ;
        theApp._ftpPasssword    = m_password ;
	    theApp._passsiveMode    = m_pasivemode;
        theApp._backupLocation   = m_selpath  = m_path;
    }
    CDialog::OnOK();

}

void CFtpSettings::LogOut(LPCTSTR psz) 
{
    m_out.SetSel(-1,-1);
        if(psz[strlen(psz)-1]!='\n')
            strcat((char*)psz,"\r\n");
    m_out.ReplaceSel(psz);
}


BOOL CFtpSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_host     =theApp._ftpIP           ;
    m_port     =theApp._ftpPort         ;
    m_login    =theApp._ftpUser         ;
    m_password =theApp._ftpPasssword    ;
    m_pasivemode=theApp._passsiveMode   ;
    m_path = m_selpath   = theApp._backupLocation;
    CheckDlgButton(IDC_CHECK1, m_pasivemode?BST_CHECKED:BST_UNCHECKED);
    HLog = &m_out;
    UpdateData(0);
	return TRUE;  
	              
}

void CFtpSettings::OnDestroy() 
{
    ftp_client.Close();
    HLog = 0;
	CDialog::OnDestroy();
}

void  CFtpSettings::PopulateTree()
{
    m_tree.SetRedraw(0);
    m_tree.DeleteAllItems();
	TV_INSERTSTRUCT tvstruct;
	int nIcon = GetIconIndex("Desktop",1);
	tvstruct.hParent = NULL;
	tvstruct.hInsertAfter    = TVI_LAST;
	tvstruct.item.iImage     = nIcon;
	tvstruct.item.iSelectedImage = nIcon;
	tvstruct.item.pszText    = _T("/");
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;

	m_tree.InsertItem(&tvstruct);    

	m_tree.SelectItem(m_tree.GetRootItem());
	m_tree.Expand(m_tree.GetRootItem(), TVE_EXPAND);


	int nIndex;
	CString strTemp;
    CString cstr;
	CString strSrc = m_path;
	
	HTREEITEM hTI = m_tree.GetRootItem();
	while (!strSrc.IsEmpty())
	{
		nIndex = strSrc.Find(_T("/"));
		if (nIndex < 0)
		{
			// last part of path
			cstr = strSrc;
			strSrc.Empty();
		}
		else 
		if (nIndex > 0)
		{ 
			// the "/" trailing the directory name is not included
			cstr = strSrc.Left(nIndex);
			strTemp = strSrc.Right(strSrc.GetLength() - (nIndex+1));
			strSrc = strTemp;
		}

		if(!cstr.IsEmpty() && (nIndex != 0))
		{
			// cstr now has the name of a file or dir -- look for a child that matches it
			hTI = m_tree.GetChildItem(hTI);
			while ((hTI != NULL) && (cstr.CompareNoCase(m_tree.GetItemText(hTI)) != 0))
			{
				hTI = m_tree.GetNextItem(hTI, TVGN_NEXT);
			}

			// if you find it, expand the node or select the file
			if ((hTI != NULL) && (cstr.CompareNoCase(m_tree.GetItemText(hTI)) == 0))
			{
				m_tree.SelectItem(hTI);
				m_tree.Expand(hTI, TVE_EXPAND);
			}
			// if it wasn't found, you've gone as far as you can, so quit...
			else 
			{	
				strSrc.Empty();
			}
		}
		else // nIndex == 0
		{
			strTemp = strSrc.Right(strSrc.GetLength() - 1);
			strSrc = strTemp;
		}
	} // end expand
    m_tree.SetRedraw(1);
    m_tree.Invalidate();

}

void CFtpSettings::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	if (pNMTreeView->hdr.code == TVN_SELCHANGED)
	{
		HTREEITEM hPrevious = m_hCurrentTreeItem;
		HTREEITEM hIT = pNMTreeView->itemNew.hItem;

		m_hCurrentTreeItem = pNMTreeView->itemNew.hItem;

		CString str, strPath;

		// build up the path to htreeitem
		strPath = m_tree.GetItemText(hIT);

		if (strPath.IsEmpty())
		{
			strPath = m_tree.GetItemText(hIT);
		}
		
		while (hIT != m_tree.GetRootItem())
		{
			hIT = m_tree.GetParentItem(hIT);
			
			if (hIT == NULL)
				break;

			if (hIT == m_tree.GetRootItem())
				str.Format(_T("/%s"), (LPCTSTR)strPath);
			else
				str.Format(_T("%s/%s"), (LPCTSTR)m_tree.GetItemText(hIT), (LPCTSTR)strPath);

			m_selpath = strPath = str;
			TRACE1("OnSelchanged: %s\n", strPath);	
		}

		// use that dir to call ExploreDir
		if (!ExploreDirectory(strPath, pNMTreeView->itemNew.hItem))
		{
			// failed to explore directory, select previous item
			m_hCurrentTreeItem = hPrevious;
			m_tree.SelectItem(m_hCurrentTreeItem);
		}
	}
    SetDlgItemText(ST_OUT,m_selpath);

    if(b_frompopulate==0)
    {
        if(IsDlgButtonChecked(CK_TIMECHECK))
            TestFileCopy();

        m_path = m_selpath;
        UpdateData(0);
    }
	*pResult = 0;
}

BOOL CFtpSettings::ExploreDirectory(const CString& strDir, HTREEITEM hParent, BOOL bRefresh)
{
	// show wait cursor
	CWaitCursor cursor; 

	if (!ftp_client.IsStillConnected())
    {

		return FALSE;
    }

    m_lbfiles.DeleteAllItems();
	if (strDir.IsEmpty())
		return FALSE;

    CString strPath;
	if (bRefresh)
	{
		HTREEITEM hItem = m_tree.GetChildItem(hParent);
		HTREEITEM hTemp;
		while (hItem != NULL)
		{
			hTemp = m_tree.GetNextSiblingItem(hItem);
			m_tree.DeleteItem(hItem);
			hItem = hTemp;
		}
	}

	BOOL bHasChilderen = m_tree.ItemHasChildren(hParent);
	if (hParent != m_tree.GetRootItem())
	{
		strPath = strDir;
	}
	else
	{
		strPath = "/";
	}

    if(!ftp_client.SetCurrentDirectory(strPath))
    {
        LogOut(MKSTR("Cannot CHange Directory To '%s'.", strPath));
        return FALSE;
    }

    m_tree.SetRedraw(0);
    m_lbfiles.SetRedraw(0);

    const vector<WIN32_FIND_DATA> vfiles = ftp_client.ListLocation(strPath);
    if(vfiles.size())
    {
        TV_INSERTSTRUCT tvstruct = {0};
        vector<WIN32_FIND_DATA>::const_iterator b = vfiles.begin(); 
        vector<WIN32_FIND_DATA>::const_iterator e = vfiles.end();

        for(;b!=e;b++)
        {
            const WIN32_FIND_DATA& fd = *b;
            if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                tvstruct.item.iImage = GetIconIndex(fd.cFileName, 1);
                tvstruct.item.iSelectedImage = GetIconIndex(fd.cFileName, 1, 1);
    			
                tvstruct.hParent       = hParent;
	    		tvstruct.hInsertAfter  = TVI_LAST;
		    	tvstruct.item.pszText  = (LPTSTR)fd.cFileName;   
			    tvstruct.item.mask     = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    			// insert item in treeview
	    		if (!bHasChilderen)
		    		m_tree.InsertItem(&tvstruct);
            }
            else 
            {
                SYSTEMTIME st;
                ::FileTimeToSystemTime(&fd.ftLastWriteTime, &st);
                m_lbfiles.AddNewItem(fd.cFileName, 
                                     MKSTR("%d/%d/%d %d:%d",st.wYear, st.wMonth, st.wDay,st.wHour,st.wMinute),
                                     MKSTR("%dKb", fd.nFileSizeLow/1024), 0);
            }
        }
    }
	LogOut(MKSTR("Listing of '%s' Received.", strPath));


    m_tree.SetRedraw(1);
    m_lbfiles.SetRedraw(1);
    m_tree.Invalidate(1);
    m_lbfiles.Invalidate(1);


	return TRUE;
}



void CFtpSettings::OnTimecheck() 
{
    if(IsDlgButtonChecked(CK_TIMECHECK))	
    {
	    if (!ftp_client.IsStillConnected())
        {
		    return ;
        }
        TestFileCopy();
    }
}
