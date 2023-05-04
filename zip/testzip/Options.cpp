// Options.cpp: implementation of the COptions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestZipDlg.h"
#include "Options.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptions::COptions()
{
	m_reg.SetInfo(_T("Options"), _T(""), this);
	SetDefaults();
}

COptions::~COptions()
{

}

void COptions::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
		ar << m_szPassword << m_iSpan << m_bTdComp << m_uVolumeSize << m_bRecurse << m_iLevel;
	else
		ar >> m_szPassword >> m_iSpan >> m_bTdComp >> m_uVolumeSize >> m_bRecurse >> m_iLevel;
}

void COptions::SetDefaults()
{
	m_iSpan = 0;
	m_bRecurse = TRUE;
	m_bTdComp = FALSE;
	m_uVolumeSize = 1457500;
	m_iLevel = -1;
	m_szPassword.Empty();
}
