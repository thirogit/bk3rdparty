// TestZipDlgDlg.h : header file
///////////////////////////////////////////////////////////////
//   Copyright (C) 2000 - 2001 Tadeusz Dracz
///////////////////////////////////////////////////////////////

#if !defined(AFX_TESTZIPDLGDLG_H__E46C0691_1EA6_4801_BD71_7C9615A0D1A7__INCLUDED_)
#define AFX_TESTZIPDLGDLG_H__E46C0691_1EA6_4801_BD71_7C9615A0D1A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestZipDlgDlg dialog
#include "ListCtrlEnh.h"
#include "Options.h"	// Added by ClassView
#include "TextProgressCtrl.h"
#include "..\ZipArchive\ZipArchive.h"	

#include "ResizableDialog.h"
struct CProgressInfo
{
	CProgressInfo()
        {
                m_iTotalSoFar = 0;
                m_iTotal = 0;
        }
	DWORD m_iTotal;
	DWORD m_iTotalSoFar;
	CTextProgressCtrl* m_pProgress;
};

struct CAddInfo
{
	CAddInfo(){}
	CAddInfo(const CString& szName, DWORD iSize, bool bDir = false)
		:m_szName(szName), m_iSize(iSize), m_bDir(bDir){}
	CString m_szName;
	bool m_bDir;
	DWORD m_iSize;
};
#include <afxtempl.h>
typedef CList<CAddInfo, CAddInfo> CAddInfoList;

class CTestZipDlgDlg : public CResizableDialog
{
// Construction
public:
	CMenu m_menu;
	bool m_bCommentModified;
	static bool ChangeDisk(int iNumber, int , void* pData);
	CTestZipDlgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestZipDlgDlg)
	enum { IDD = IDD_TESTZIPDLG_DIALOG };
	CTextProgressCtrl m_progress;
	CListCtrlEnh	m_files;
	CString	m_szComment;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestZipDlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static bool Callback(int , int iSoFar, void* pData);
	void UpdateMenu(CMenu* pMenu);
	void AddFolder(CString szFolder, CAddInfoList& list, CProgressInfo& p);
	void DisplayLabel();
	void Redisplay();
	bool GetFileName(CString &szFile, bool bOpen);
	CZipArchive m_zip;
	COptions m_options;
	HICON m_hIcon;
	bool m_bAfterException ;

	// Generated message map functions
	//{{AFX_MSG(CTestZipDlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnActionAddFiles();
	afx_msg void OnActionAddFolder();
	afx_msg void OnActionDelete();
	afx_msg void OnActionExtract();
	afx_msg void OnArchiveClose();
	afx_msg void OnArchiveExit();
	afx_msg void OnArchiveNew();
	afx_msg void OnArchiveOpen();
	afx_msg void OnArchiveOptions();
	afx_msg void OnHelpAbout();
	afx_msg void OnUpdateActionAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActionDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActionExtract(CCmdUI* pCmdUI);
	afx_msg void OnUpdateArchiveClose(CCmdUI* pCmdUI);
	afx_msg void OnChangeEdit1();
	afx_msg void OnDblclkContents(NMHDR* , LRESULT* pResult);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnActionTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTZIPDLGDLG_H__E46C0691_1EA6_4801_BD71_7C9615A0D1A7__INCLUDED_)
