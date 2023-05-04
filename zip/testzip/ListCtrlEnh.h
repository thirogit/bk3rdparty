#if !defined(AFX_LISTCTRLENH_H__64C3C120_E937_11D3_B7C7_8AE1A0423842__INCLUDED_)
#define AFX_LISTCTRLENH_H__64C3C120_E937_11D3_B7C7_8AE1A0423842__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEnh.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEnh window

class CListCtrlEnh : public CListCtrl
{
// Construction
public:
	CListCtrlEnh();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEnh)
	//}}AFX_VIRTUAL

// Implementation
public:
	void BuildSelectedArray(CDWordArray& dwArray);
	void UpdateColumnWidths();
	int InsertColumn(int nCol, UINT nIDColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	int GetFirstSelectedItem();
	virtual ~CListCtrlEnh();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEnh)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLENH_H__64C3C120_E937_11D3_B7C7_8AE1A0423842__INCLUDED_)
