// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestZipDlg.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	//}}AFX_DATA_INIT
	m_pOptions = NULL;
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO1, m_pOptions->m_iSpan);
	DDX_Check(pDX, IDC_CHECK1, m_pOptions->m_bTdComp);
	DDX_Check(pDX, IDC_CHECK2, m_pOptions->m_bRecurse);
	DDX_Text(pDX, IDC_EDIT1, m_pOptions->m_uVolumeSize);
	DDX_Text(pDX, IDC_EDIT2, m_pOptions->m_iLevel);
	DDX_Text(pDX, IDC_EDIT3, m_pOptions->m_szPassword);

}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

void COptionsDlg::OnOK() 
{
	CDialog::OnOK();
	m_pOptions->m_reg.Save();
}

void COptionsDlg::OnRadio() 
{
	CButton* p = (CButton*)GetDlgItem(IDC_RADIO3);
	m_edit.EnableWindow(p->GetCheck());
}

BOOL COptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_spin.SetRange(-1, 9);	
	OnRadio();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
