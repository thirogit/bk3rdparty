#if !defined(AFX_PAGEHYPERLINKS_H__35FC4F1E_1F84_4A8C_8469_7803DF2EB8C9__INCLUDED_)
#define AFX_PAGEHYPERLINKS_H__35FC4F1E_1F84_4A8C_8469_7803DF2EB8C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageHyperLinks.h : header file
//
#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageHyperLinks dialog

class CPageHyperLinks : public CPageBase
{
// Construction
public:
	CPageHyperLinks(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageHyperLinks)
	enum { IDD = IDD_PAGE_HYPER_LINKS };
	CExtComboBox	m_wndLinkCursor;
	CExtFlatEdit	m_wndLinkURL;
	CExtFlatEdit	m_wndLinkCaption;
	CExtFlatEdit	m_wndLinkTooltip;
	CExtLabel	m_wndLabelURL;
	CExtLabel	m_wndLabelCaption;
	CExtLabel	m_wndLabelTooltip;
	CExtLabel	m_wndLabelCursor;
	CExtCheckBox	m_wndLinkUnderline;
	CExtCheckBox	m_wndLinkShowFocus;
	CExtCheckBox	m_wndLinkEnable;
	CExtColorButton m_wndLinkColorVisited;
	CExtColorButton	m_wndLinkColorHover;
	CExtColorButton	m_wndLinkColor;
	CExtHyperLinkButton	m_wndHyperLink2;
	CExtHyperLinkButton	m_wndHyperLink1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageHyperLinks)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;

	// Generated message map functions
	//{{AFX_MSG(CPageHyperLinks)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokLinkCursor();
	//}}AFX_MSG
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEHYPERLINKS_H__35FC4F1E_1F84_4A8C_8469_7803DF2EB8C9__INCLUDED_)
