// RegistrySerialize.h: interface for the CRegistrySerialize class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 - 2001 Tadeusz Dracz.
//  For conditions of distribution and use, see the copyright notice in License.txt
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRYSERIALIZE_H__50F70D46_D431_4414_9777_1360E38400E6__INCLUDED_)
#define AFX_REGISTRYSERIALIZE_H__50F70D46_D431_4414_9777_1360E38400E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlbase.h>
class CRegistrySerialize  
{
public:
	bool Save();
	bool Load();
	void SetInfo(CString szSection, LPCTSTR lpszValue, CObject* pObject);
	CRegistrySerialize();
	virtual ~CRegistrySerialize();

protected:
	bool Check();
	CObject* m_pObject;
	CString m_szValue;
	CRegKey m_Key;
};

#endif // !defined(AFX_REGISTRYSERIALIZE_H__50F70D46_D431_4414_9777_1360E38400E6__INCLUDED_)
