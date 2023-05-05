// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__A274CAD4_2CB1_4E8A_9B59_6F178266DEF8__INCLUDED_)
#define AFX_MAINFRM_H__A274CAD4_2CB1_4E8A_9B59_6F178266DEF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#if !defined(__EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif


class CMainFrame : public CExtNCW < CFrameWnd >
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	WINDOWPLACEMENT m_dataFrameWP;

public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void ActivateFrame(int nCmdShow = -1);
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CExtMenuControlBar    m_wndMenuBar;
	CExtStatusControlBar  m_wndStatusBar;
	CExtToolControlBar    m_wndToolBar;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;
	CExtControlBar m_wndResizableBarEmpty;
	CExtControlBar m_wndResizableBarTree;
	
	CExtNCSB < CExtWFF < CTreeCtrl > > m_wndDockedCtrlTree;
	CExtControlBar m_wndResizableBarEdit;
	CExtNCSB < CExtEdit > m_wndDockedCtrlEdit;
	CExtControlBar m_wndResizableBarCP;
	CExtWRB< CExtColorCtrl > m_wndDockedCtrlCP;
	CExtControlBar m_wndResizableBarDlg;
	CExtWRB< CExtResizableDialog > m_wndDockedResizableDialog;
	CExtWRB< CExtEdit > m_wndEditInDockedDlg;
	CExtLabel m_wndLableInDockedDlg;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__A274CAD4_2CB1_4E8A_9B59_6F178266DEF8__INCLUDED_)
