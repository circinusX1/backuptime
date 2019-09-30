//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include "backupiime.h"
#include "FolderTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderTree

CFolderTree::CFolderTree()
{

}

CFolderTree::~CFolderTree()
{
}


BEGIN_MESSAGE_MAP(CFolderTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CFolderTree)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderTree message handlers

void CFolderTree::PreSubclassWindow() 
{
	HIMAGELIST himlNormal;
	SHFILEINFO sfi;

	himlNormal = (HIMAGELIST) SHGetFileInfo ((LPCSTR) "C:\\", 
		0, &sfi, sizeof (SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	if (himlNormal)
	{
		::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM)TVSIL_NORMAL, (LPARAM)himlNormal);
	}
	
	CTreeCtrl::PreSubclassWindow();
}

HTREEITEM CFolderTree::FindItem(LPCTSTR lpszText, HTREEITEM hItem)
{
	HTREEITEM htiSelected = hItem ? hItem : GetSelectedItem();
	HTREEITEM htiCurrent = GetNextItemEx(htiSelected);
	
	if(htiCurrent == NULL)
	{
		htiCurrent = GetRootItem();
	}

	while(htiCurrent && htiCurrent != htiSelected)
	{
		CString strItemText = GetItemText(htiCurrent);

		if (strItemText.Compare(lpszText) == 0)
		{
			// found string
			return htiCurrent;
		}

		// get next
		htiCurrent = GetNextItemEx(htiCurrent);
		if(htiCurrent == NULL && htiSelected != NULL)
		{
			htiCurrent = GetRootItem();
		}
	}
	return NULL;
}


HTREEITEM CFolderTree::GetNextItemEx(HTREEITEM hItem)
{
	HTREEITEM hti;

	if(ItemHasChildren(hItem))
	{
		// return first child
		return GetChildItem(hItem);           
	}
	else
	{
		// return next sibling item
        // Go up the tree to find a parent's sibling if needed.
        while((hti = GetNextSiblingItem(hItem)) == NULL)
		{
			if((hItem = GetParentItem(hItem)) == NULL)
				return NULL;
		}
	}
    return hti;
}


