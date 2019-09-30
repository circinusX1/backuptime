//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include "Resource.h"								
#include "DlgFolderSelect.h"								


static  TCHAR*	DupString( LPTSTR szData );
static CString WindowName( CString sName );
static bool	ParsPath( CString sPath, int nLevel, CString *const psDirName );

#define NETWORK_NEIGHBOUR   _T("Network Neighborhood")


CDlgFolderSelect::CDlgFolderSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFolderSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFolderSelect)
	//}}AFX_DATA_INIT
	m_hNetworkRoot = NULL;
    b_localdrives = 1;
    b_mappeddrives = 1;
    b_hideFileTypes = 0;

}


void CDlgFolderSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFolderSelect)
	DDX_Control(pDX, IDC_TREE1, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFolderSelect, CDialog)
	//{{AFX_MSG_MAP(CDlgFolderSelect)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, OnItemexpandingTree)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

bool ParsPath( CString sPath, int nLevel, CString *const psDirName )
{
	ASSERT( psDirName );
	//find first match
	int nStart = 0;									//Location in sPath to search from
	while( nLevel )
	{
		nStart = sPath.Find( _T('\\'), nStart );
		if( nStart < 0 )
			return false;
		nLevel--;
		nStart++;
	}
	//We now have the start point to find the end.
	int nEnd = sPath.Find( _T('\\'), nStart );
	if( nEnd < 0 )
		*psDirName = sPath.Mid( nStart );
	else
		*psDirName = sPath.Mid( nStart, nEnd-nStart);
	return true;
}

BOOL CDlgFolderSelect::OnInitDialog() 
{
	TRACE( _T("CDlgFolderSelect::OnInitDialog()\n") );
	CDialog::OnInitDialog();

	if( !m_ImageListTree.Create( IDB_FILE_TYPES, 17, 1, RGB( 255, 255, 255 ) ) )
		TRACE( _T("Image list creation fault") );
	m_Tree.SetImageList( &m_ImageListTree, TVSIL_NORMAL );

    if(b_localdrives)
    {
	    // List the local drives
	    for( TCHAR cLetter = _T('A'); cLetter <= _T('Z'); cLetter++ )
	    {
		    CString sDrive = cLetter;
		    sDrive += _T(":");						
		    UINT nType = GetDriveType( sDrive + _T("\\") );
		    if( DRIVE_REMOVABLE <= nType && nType <= DRIVE_RAMDISK )
			    InsertItem( TVI_ROOT, NULL, sDrive, nType );
	    }
    }

    if(b_mappeddrives)
    {
    	m_hNetworkRoot = InsertItem( TVI_ROOT, NULL, NETWORK_NEIGHBOUR, DRIVE_RAMDISK+1 );
    }

	
    // get initial path selection and file filters 
    if(m_fileFilters.GetLength()==0)
    {
        m_fileFilters = "*.*";
    }
    SetDlgItemText(EF_FTYPES,m_fileFilters);

	//Expand the Tree to the Inital Path.
	//
	int nLevel = 0;
	CString sDirName;
	HTREEITEM hCurrent = TVI_ROOT;					//Current item in the list being expanded.
	while( ParsPath( m_sPath, nLevel, &sDirName ) )
	{
		//Network or Drive expand
		if( !nLevel && sDirName.IsEmpty() )
				sDirName = NETWORK_NEIGHBOUR;
		//Search for the matching tree item and expand
		HTREEITEM hItem = m_Tree.GetChildItem( hCurrent );	
		while( hItem )
		{
			if( sDirName.CompareNoCase( m_Tree.GetItemText( hItem ) ) == 0 )
			{
				hCurrent = hItem;
				m_Tree.Expand( hCurrent, TVE_EXPAND );
				break;
			}
			hItem = m_Tree.GetNextSiblingItem( hItem );
		}
		nLevel++;
	}

    SetWindowText(m_Title);
    if(b_hideFileTypes)
    {
        GetDlgItem(EF_FTYPES)->ShowWindow(SW_HIDE);
    }

	return TRUE;  
	              
}

HTREEITEM CDlgFolderSelect::InsertItem( HTREEITEM hParent, NETRESOURCE *const pNetResource, CString sText, int iImage, int iImageSelected /* = -1 */ )
{
	TRACE( _T("CDlgFolderSelect::InsertItem(%p,%p,%s +++ )\n"), hParent, pNetResource, sText );
	sText = WindowName( sText );
	TVINSERTSTRUCT InsertStruct;
	InsertStruct.hParent		= hParent;	
	InsertStruct.hInsertAfter	= TVI_LAST;
	InsertStruct.itemex.mask	= TVIF_IMAGE | TVIF_TEXT | TVIF_CHILDREN | 
								  TVIF_SELECTEDIMAGE | TVIF_PARAM;
	InsertStruct.itemex.pszText	= sText.GetBuffer( sText.GetLength() );  sText.ReleaseBuffer();
	InsertStruct.itemex.iImage	= iImage;
	InsertStruct.itemex.cChildren= 1;
	InsertStruct.itemex.lParam	= (LPARAM)pNetResource;
	if( iImageSelected == -1 )
		InsertStruct.itemex.iSelectedImage = iImage;
	else
		InsertStruct.itemex.iSelectedImage = iImageSelected;
	return m_Tree.InsertItem( &InsertStruct );
}

bool CDlgFolderSelect::PopulateTree( CString sPath, HTREEITEM hParent ) 
{
	TRACE( _T("CDlgFolderSelect::PopulateTree( %s )\n"), sPath );
	bool bGotChildren = false;						//True if a child is added.	
	//
	//Populate Network neighbourhood tree (Entire network and Local-Computers)
	//
	if( hParent == m_hNetworkRoot )
	{
		bGotChildren = EnumNetwork( hParent );
	}
	//
	//Network item(Search deep into the network)
	//
	else if( m_Tree.GetItemData( hParent ) )
	{
		bGotChildren = EnumNetwork( hParent );
	}
	
	//
	//Search for files and populate the CTreeCtrl
	//
	else
	{
		CFileFind finder;
		BOOL bWorking = finder.FindFile( sPath+_T("*.*") );
		while (bWorking)   
		{
			bWorking = finder.FindNextFile();
			if( finder.IsDots() )
				continue;
			if( finder.IsDirectory() )
			{
				InsertItem( hParent, NULL, finder.GetFileName(), DRIVE_NO_ROOT_DIR, DRIVE_UNKNOWN );
				bGotChildren = true;
			}
		}
	}

	//
	//Remove the [+] if no children
	//
	if( !bGotChildren )
	{
		TVITEM item		= { 0 };
		item.mask		= TVIF_HANDLE | TVIF_CHILDREN;
		item.hItem		= hParent;
		item.cChildren	= 0;
		m_Tree.SetItem( &item );
	}
	return bGotChildren;
}

CString CDlgFolderSelect::GetItemPath( HTREEITEM hItem ) 
{
	TRACE( _T("CDlgFolderSelect::GetItemPath(%p)\n"), hItem );
	CString sRet;
	do
	{
		//End with a share name.
		NETRESOURCE *const pNetResource = (NETRESOURCE *)(m_Tree.GetItemData( hItem ) );
		if( pNetResource )
		{
			sRet = CString(pNetResource->lpRemoteName) + _T('\\')+ sRet; 		
			break;
		}
		//Add the directory name to the path.
		sRet = m_Tree.GetItemText( hItem ) + _T('\\')+ sRet; 
		hItem = m_Tree.GetParentItem( hItem );
	} while( hItem );
	return sRet;
}


void CDlgFolderSelect::OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE( _T("CDlgFolderSelect::OnItemexpandingTree(%p)\n"), pNMHDR );
	CWaitCursor CursorWaiting;						//Show the wait cursor while expanding
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	ASSERT( pNMTreeView );
	ASSERT( pResult );
	//Only action 2 notifications
	if( pNMTreeView->action == 2 )
	{
		//Update location display
		CString sPath = GetItemPath( pNMTreeView->itemNew.hItem );
		//Refresh children
		if( !m_Tree.GetChildItem( pNMTreeView->itemNew.hItem ) )
		{
			PopulateTree( sPath, pNMTreeView->itemNew.hItem );
			if( m_Tree.GetSelectedItem( ) != pNMTreeView->itemNew.hItem )
				m_Tree.SelectItem( pNMTreeView->itemNew.hItem );
		}
	}
	*pResult = 0;
}


void CDlgFolderSelect::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE( _T("CDlgFolderSelect::OnSelchangedTree(%p)\n"), pNMHDR );
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	ASSERT( pNMTreeView );
	ASSERT( pResult );
	CString sPath;									//Currently selected path or empty if not valid
	//Disable search on Workstation roots
	if( m_Tree.GetItemData( pNMTreeView->itemNew.hItem ) || 
			pNMTreeView->itemNew.hItem == m_hNetworkRoot )
	{
		GetDlgItem( IDOK )->EnableWindow( false );
	}
	else
	{
		GetDlgItem( IDOK )->EnableWindow( true );
		sPath = GetItemPath( pNMTreeView->itemNew.hItem );
	}
	*pResult = 0;
	
	SetDlgItemText( IDC_STATIC_NOTE, sPath );
}

					
TCHAR* DupString( LPTSTR szData )
{
	TRACE( _T("DupString( %s )\n"), szData );
	//Assume a NULL = empty string
	TCHAR * szRet = NULL;
	int nLength	  = 0;
	if( szData )
		nLength = _tcslen( szData )+1;
	if( nLength > 0 )
	{
		szRet = new TCHAR[nLength];
		ASSERT( szRet );
		_tcscpy( szRet, szData );
	}
	return szRet;
}


CString WindowName( CString sName )
{
	CString sRet;
	//Scan all charactors to determine if their are any lower case items
	for( int n = 0; n < sName.GetLength(); n++ )
	{
		TCHAR ch = sName[n];
		if ((ch >= 'a') && (ch <= 'z') )
			return sName;
	}
	sName.MakeLower();
	if( sName.GetLength() > 0 )
	{
		CString sFirstChar = sName[0];
		sFirstChar.MakeUpper();
		sName = sFirstChar + sName.Mid( 1 );
	}
	return sName;
}

bool CDlgFolderSelect::EnumNetwork( HTREEITEM hParent )
{
	TRACE( _T("CDlgFolderSelect::EnumNetwork( %p )\n"), hParent );
	bool bGotChildren = false;						//True if a child is added.	

	//Check if the item already has a network resource and use it.
	NETRESOURCE *const pNetResource = (NETRESOURCE *)(m_Tree.GetItemData( hParent ) );
	//
	//Setup
	//
	DWORD dwResult;      
	HANDLE hEnum;      
	DWORD cbBuffer = 16384;
	DWORD cEntries = 0xFFFFFFFF;      
	LPNETRESOURCE lpnrDrv;      
	DWORD i;
	dwResult = WNetOpenEnum(	pNetResource ?  RESOURCE_GLOBALNET  : RESOURCE_CONTEXT,
								RESOURCETYPE_ANY,//RESOURCETYPE_DISK,
								0,                               
								pNetResource ? 	pNetResource		: NULL,
								&hEnum );      
	//Was the read sucessfull
	if (dwResult != NO_ERROR)      
	{
		TRACE( _T("*** ERROR %d - Cannot enumerate network drives.\n"), dwResult );
		return false;
	} 
	
	//
	//Get items until no more remain.
	//
	do      
	{
		lpnrDrv = (LPNETRESOURCE) GlobalAlloc( GPTR, cbBuffer );
		dwResult = WNetEnumResource( hEnum, &cEntries, lpnrDrv, &cbBuffer   );
		if (dwResult == NO_ERROR)         
		{
			//Scann through the results
			for( i = 0; i < cEntries; i++ )            
			{
				CString sNameRemote = lpnrDrv[i].lpRemoteName;
				int nType = 9;
				if( sNameRemote.IsEmpty() )
				{
					sNameRemote = lpnrDrv[i].lpComment;
					nType = 8;
				}
				//
				//Remove leading back slashes 
				//
				if( sNameRemote.GetLength() > 0 && sNameRemote[0] == _T('\\') )
					sNameRemote = sNameRemote.Mid( 1 );
				if( sNameRemote.GetLength() > 0 && sNameRemote[0] == _T('\\') )
					sNameRemote = sNameRemote.Mid( 1 );

				//
				//Display a share or the appropiate icon
				//
				if( lpnrDrv[i].dwDisplayType == RESOURCEDISPLAYTYPE_SHARE )
				{
					//Display only the share name
					int nPos = sNameRemote.Find( _T('\\') );
					if( nPos >= 0 )
						sNameRemote = sNameRemote.Mid( nPos+1 );
					InsertItem( hParent, NULL, sNameRemote, DRIVE_NO_ROOT_DIR, DRIVE_UNKNOWN );
				}
				else
				{
					NETRESOURCE* pResource = new NETRESOURCE;
					ASSERT( pResource );
					*pResource = lpnrDrv[i];
					pResource->lpLocalName	= DupString( pResource->lpLocalName );
					pResource->lpRemoteName = DupString( pResource->lpRemoteName );
					pResource->lpComment	= DupString( pResource->lpComment );
					pResource->lpProvider	= DupString( pResource->lpProvider );
					InsertItem( hParent, pResource, sNameRemote, pResource->dwDisplayType+7 );
				}
				bGotChildren = true;
			}
		}         
		GlobalFree( (HGLOBAL) lpnrDrv );      
		if( dwResult != ERROR_NO_MORE_ITEMS )
		{            
			TRACE( _T("*** ERROR %d - Cannot complete network drive enumeration\n"), dwResult );
			break;         
		}
	}
	while( dwResult != ERROR_NO_MORE_ITEMS );      

	//
	//Let go and go home
	//
	WNetCloseEnum(hEnum);   
	return bGotChildren;
}


BOOL CDlgFolderSelect::DestroyWindow() 
{
	TRACE( _T("CDlgFolderSelect::DestroyWindow()\n") );
	//Get the current selection before heading home


	//Start at the root deleting.
	HTREEITEM hItemCur = m_Tree.GetRootItem( );		//Current item under inspection

	//
	//While their is still data in the tree
	//
	while( hItemCur )
	{
		HTREEITEM hItem = m_Tree.GetChildItem( hItemCur );
		//Has children then make the child current
		if( hItem )
		{
			hItemCur = hItem;
			continue;
		}

		//Item has not children so we shall destroy it.
		//but first we must decide who is to take its place.
		HTREEITEM hNextSibItem = m_Tree.GetNextSiblingItem( hItemCur );
		HTREEITEM hPrevSibItem = m_Tree.GetPrevSiblingItem( hItemCur );
		HTREEITEM hParentItem  = m_Tree.GetParentItem( hItemCur );
		//Get item data to check if lparam is to be destroyed
		NETRESOURCE *const pNetResource = (NETRESOURCE *)m_Tree.GetItemData( hItemCur );//(item.lParam);
		if( pNetResource )
		{
			delete [] (pNetResource->lpLocalName);
			delete [] (pNetResource->lpRemoteName);
			delete [] (pNetResource->lpComment);
			delete [] (pNetResource->lpProvider);
			delete pNetResource;
		}
		m_Tree.DeleteItem( hItemCur );
		hItemCur = NULL;
		//Determine which item is next to recieve the focus
		if( hParentItem )
			hItemCur = hParentItem;
		if( hPrevSibItem )
			hItemCur = hPrevSibItem;
		if( hNextSibItem )
			hItemCur = hNextSibItem;
	}	
	//All items removed from list now. Lets trash this place and go home.
	return CDialog::DestroyWindow();
}


void CDlgFolderSelect::OnOk() 
{
    m_sPath = GetItemPath( m_Tree.GetSelectedItem( ) );
    GetDlgItemText(EF_FTYPES, m_fileFilters);	
    CDialog::OnOK();
}

void CDlgFolderSelect::OnCancel() 
{
	CDialog::OnCancel();
}
