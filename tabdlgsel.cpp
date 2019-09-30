//-----------------------------------------------------------------------------
// Author: Marius Chincisan 2005
#include "stdafx.h"
#include "resource.h"
#include "TabDlgSel.h"
#include "BackUpiImeDlg.h"

TabDlgSel::TabDlgSel()
{
}

TabDlgSel::~TabDlgSel()
{
}


BEGIN_MESSAGE_MAP(TabDlgSel, CTabCtrl)
	//{{AFX_MSG_MAP(TabDlgSel)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void TabDlgSel::AddDialog(CDialog * pDlg, LPCSTR pTitle,int iMage)
{
	TC_ITEM tci={TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM,0,0,NULL,32,0,NULL};
	tci.pszText = (LPSTR)pTitle;
	tci.iImage	= iMage;
	tci.lParam  = (LPARAM)pDlg;
    
    
    InsertItem(GetItemCount(),&tci);
}

void TabDlgSel::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char text[32];
	TC_ITEM tci={TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM,0,0,text,32,0,NULL};
	GetItem(GetCurSel(), &tci);

    CDialog* pDlg = ((CDialog*)tci.lParam);

    RECT rt;
    _pParent->GetDlgItem(ST_DLG)->GetWindowRect(&rt);
    _pParent->ScreenToClient(&rt);
    pDlg->MoveWindow(rt.left,rt.top,rt.right-rt.left, rt.bottom-rt.top,1);

    pDlg->ShowWindow(SW_SHOW);

    _pParent->PostMessage(WM_PB_APPLY);
	*pResult = 0;	
}

void TabDlgSel::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char text[32];
	TC_ITEM tci={TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM,0,0,text,32,0,NULL};
	GetItem(GetCurSel(), &tci);
    CDialog* pDlg = ((CDialog*)tci.lParam);
	pDlg->ShowWindow(SW_HIDE);
	*pResult = 0;
}

