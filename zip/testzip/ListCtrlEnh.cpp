// ListCtrlEnh.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEnh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEnh

CListCtrlEnh::CListCtrlEnh()
{
}

CListCtrlEnh::~CListCtrlEnh()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEnh, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEnh)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEnh message handlers

// get the index of the first selected item
int CListCtrlEnh::GetFirstSelectedItem()
{
	POSITION pos = GetFirstSelectedItemPosition();
	if (!pos)
		return -1;
	return GetNextSelectedItem(pos);

}

// insert column with the string ID
int CListCtrlEnh::InsertColumn(int nCol, UINT nIDColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	CString col;
	col.LoadString(nIDColumnHeading);
	return CListCtrl::InsertColumn(nCol, col, nFormat, nWidth, nSubItem);
}

void CListCtrlEnh::UpdateColumnWidths()
{
	SetRedraw(FALSE);
	int width = (GetItemCount() ? LVSCW_AUTOSIZE : LVSCW_AUTOSIZE_USEHEADER);
	for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
		SetColumnWidth(i, width);
	SetRedraw();
}

void CListCtrlEnh::BuildSelectedArray(CDWordArray &dwArray)
{
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos)
		dwArray.Add(GetNextSelectedItem(pos));

}
