// TestZipDlg.h : main header file for the TESTZIPDLG application
//

#if !defined(AFX_TESTZIPDLG_H__F84D06CB_6616_471D_8F76_027B2B634FDE__INCLUDED_)
#define AFX_TESTZIPDLG_H__F84D06CB_6616_471D_8F76_027B2B634FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestZipDlgApp:
// See TestZipDlg.cpp for the implementation of this class
//

class CTestZipDlgApp : public CWinApp
{
public:
	CTestZipDlgApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestZipDlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestZipDlgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTZIPDLG_H__F84D06CB_6616_471D_8F76_027B2B634FDE__INCLUDED_)
