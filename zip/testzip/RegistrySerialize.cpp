// RegistrySerialize.cpp: implementation of the CRegistrySerialize class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 - 2001 Tadeusz Dracz.
//  For conditions of distribution and use, see the copyright notice in License.txt
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestZipDlg.h"
#include "RegistrySerialize.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistrySerialize::CRegistrySerialize()
{
	m_pObject = NULL;
}

CRegistrySerialize::~CRegistrySerialize()
{

}

void CRegistrySerialize::SetInfo(CString szSection, LPCTSTR lpszValue, CObject* pObject)
{
	m_Key.Close();
	m_Key.Attach(AfxGetApp()->GetSectionKey(szSection));
	m_pObject = pObject;
	m_szValue = lpszValue;
}



bool CRegistrySerialize::Load()
{
	if (!Check())
		return false;
	LPBYTE pData = NULL;
	bool ret = false;
	try
	{
	
		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(m_Key, m_szValue, NULL, &dwType, NULL, &dwCount);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			pData = new BYTE[dwCount];
			lResult = RegQueryValueEx(m_Key, m_szValue, NULL, &dwType, pData, &dwCount);
		}


		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			CMemFile mf(pData, dwCount);
			CArchive ar(&mf, CArchive::load);
			m_pObject->Serialize(ar);
			ret = true;
		}

	}
	catch(CException*e)
	{
		e->Delete();
// 		if (pData)
			delete[]pData;
		return false;
	}
// 	if (pData)
		delete[]pData;
	return ret;

}

bool CRegistrySerialize::Save()
{
	if (!Check())
		return false;

	CMemFile mf;
	try
	{
	
		CArchive ar(&mf, CArchive::store);
		m_pObject->Serialize(ar);
		ar.Close();
	}
	catch(CException* e)
	{
		e->Delete();
		return false;
	}

	int iSize = mf.GetLength();
	BYTE* pData = mf.Detach();
	bool ret = RegSetValueEx(m_Key, m_szValue, 0, REG_BINARY, pData, iSize) == ERROR_SUCCESS;
	free(pData);
	return ret;
}

bool CRegistrySerialize::Check()
{
	return m_Key.m_hKey && m_pObject;
}
