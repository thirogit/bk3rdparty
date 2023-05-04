// CommentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestZipDlg.h"
#include "CommentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommentDlg dialog


CCommentDlg::CCommentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommentDlg)
	m_szComment = _T("");
	//}}AFX_DATA_INIT
}


void CCommentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommentDlg)
	DDX_Text(pDX, IDC_EDIT1, m_szComment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommentDlg, CDialog)
	//{{AFX_MSG_MAP(CCommentDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommentDlg message handlers
