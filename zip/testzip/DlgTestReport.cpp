// DlgTestReport.cpp : implementation file
//

#include "stdafx.h"
#include "TestZipDlg.h"
#include "DlgTestReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTestReport dialog


CDlgTestReport::CDlgTestReport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTestReport)
	//}}AFX_DATA_INIT
	m_pszData = NULL;
}


void CDlgTestReport::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(m_pszData);
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTestReport)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT1, *m_pszData);
}


BEGIN_MESSAGE_MAP(CDlgTestReport, CDialog)
	//{{AFX_MSG_MAP(CDlgTestReport)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTestReport message handlers
