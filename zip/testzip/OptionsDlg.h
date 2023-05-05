#if !defined(AFX_OPTIONSDLG_H__04F5BF15_EE44_47CB_A25F_84B722E0FF10__INCLUDED_)
#define AFX_OPTIONSDLG_H__04F5BF15_EE44_47CB_A25F_84B722E0FF10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog
#include "Options.h"
class COptionsDlg : public CDialog
{
// Construction
public:
	COptions* m_pOptions;
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS };
	CSpinButtonCtrl	m_spin;
	CEdit	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual void OnOK();
	afx_msg void OnRadio();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__04F5BF15_EE44_47CB_A25F_84B722E0FF10__INCLUDED_)
