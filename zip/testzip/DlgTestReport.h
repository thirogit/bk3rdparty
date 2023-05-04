#if !defined(AFX_DLGTESTREPORT_H__2746D6EB_27BE_45C9_B25E_E798BF9D54DE__INCLUDED_)
#define AFX_DLGTESTREPORT_H__2746D6EB_27BE_45C9_B25E_E798BF9D54DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTestReport.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTestReport dialog

class CDlgTestReport : public CDialog
{
// Construction
public:
	CDlgTestReport(CWnd* pParent = NULL);   // standard constructor
	CString*	m_pszData;
// Dialog Data
	//{{AFX_DATA(CDlgTestReport)
	enum { IDD = IDD_TEST_REPORT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTestReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTestReport)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTESTREPORT_H__2746D6EB_27BE_45C9_B25E_E798BF9D54DE__INCLUDED_)
