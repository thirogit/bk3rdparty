// Options.h: interface for the COptions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONS_H__8AE205B8_2773_40C4_8A59_40D29A7386F5__INCLUDED_)
#define AFX_OPTIONS_H__8AE205B8_2773_40C4_8A59_40D29A7386F5__INCLUDED_

#include "RegistrySerialize.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COptions : public CObject
{
public:
	// -1 : default
	// 0 - store
	// 1-9 - deflate levels
	int m_iLevel;
	void SetDefaults();
	BOOL m_bRecurse;
	void Serialize( CArchive& ar );
	int		m_iSpan;
	// when opening an existing archive on a removable device, assume it is td span compatible
	BOOL	m_bTdComp;
	// the size of the volume in td span mode
	DWORD	m_uVolumeSize;
	CString m_szPassword;
	COptions();
	virtual ~COptions();
	CRegistrySerialize m_reg;
};

#endif // !defined(AFX_OPTIONS_H__8AE205B8_2773_40C4_8A59_40D29A7386F5__INCLUDED_)
