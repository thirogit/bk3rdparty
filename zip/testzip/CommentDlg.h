#if !defined(AFX_COMMENTDLG_H__12E8A4EB_D9B0_4D2C_87AE_3863BBB9FED1__INCLUDED_)
#define AFX_COMMENTDLG_H__12E8A4EB_D9B0_4D2C_87AE_3863BBB9FED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommentDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommentDlg dialog

class CCommentDlg : public CDialog
{
// Construction
public:
	CCommentDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommentDlg)
	enum { IDD = IDD_FILECOMMENT };
	CString	m_szComment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommentDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMENTDLG_H__12E8A4EB_D9B0_4D2C_87AE_3863BBB9FED1__INCLUDED_)
