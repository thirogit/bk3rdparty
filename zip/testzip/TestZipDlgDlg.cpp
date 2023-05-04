// TestZipDlgDlg.cpp : implementation file
///////////////////////////////////////////////////////////////
//   Copyright (C) 2000 - 2001 Tadeusz Dracz
///////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "TestZipDlg.h"
#include "TestZipDlgDlg.h"
#include "OptionsDlg.h"
#include "BrowseForFolder.h"
#include "AboutDlg.h"
#include "CommentDlg.h"
#include "DlgTestReport.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTestZipDlgDlg dialog


CTestZipDlgDlg::CTestZipDlgDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CTestZipDlgDlg::IDD, pParent)
{

	//{{AFX_DATA_INIT(CTestZipDlgDlg)
	m_szComment = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bAfterException = false;
}

void CTestZipDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestZipDlgDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_CONTENTS, m_files);
	DDX_Text(pDX, IDC_EDIT1, m_szComment);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestZipDlgDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CTestZipDlgDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_COMMAND(ID_ACTION_ADD_FILES, OnActionAddFiles)
	ON_COMMAND(ID_ACTION_ADD_FOLODER, OnActionAddFolder)
	ON_COMMAND(ID_ACTION_DELETE, OnActionDelete)
	ON_COMMAND(ID_ACTION_EXTRACT, OnActionExtract)
	ON_COMMAND(ID_ARCHIVE_CLOSE, OnArchiveClose)
	ON_COMMAND(ID_ARCHIVE_EXIT, OnArchiveExit)
	ON_COMMAND(ID_ARCHIVE_NEW, OnArchiveNew)
	ON_COMMAND(ID_ARCHIVE_OPEN, OnArchiveOpen)
	ON_COMMAND(ID_ARCHIVE_OPTIONS, OnArchiveOptions)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ADD_FILES, OnUpdateActionAdd)
	ON_UPDATE_COMMAND_UI(ID_ACTION_DELETE, OnUpdateActionDelete)
	ON_UPDATE_COMMAND_UI(ID_ACTION_EXTRACT, OnUpdateActionExtract)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVE_CLOSE, OnUpdateArchiveClose)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_NOTIFY(NM_DBLCLK, IDC_CONTENTS, OnDblclkContents)
	ON_WM_INITMENU()
	ON_UPDATE_COMMAND_UI(ID_ACTION_ADD_FOLODER, OnUpdateActionAdd)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TEST, OnUpdateActionExtract)
	ON_COMMAND(ID_ACTION_TEST, OnActionTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestZipDlgDlg message handlers

BOOL CTestZipDlgDlg::OnInitDialog()
{
	m_zip.SetSpanCallback(ChangeDisk, this);
	m_zip.SetAdvanced(1500000);
	if (!m_options.m_reg.Load())
		m_options.SetDefaults();
// 	m_zip.SetConvertAfterOpen(false);

	CResizableDialog::OnInitDialog();
	UpdateDialogControls(this, FALSE);

	m_files.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_files.InsertColumn(0, IDS_COL1);
	m_files.InsertColumn(1, IDS_COL2);
	m_files.InsertColumn(2, IDS_COL3, LVCFMT_RIGHT);
	m_files.InsertColumn(3, IDS_COL4);
	m_files.InsertColumn(4, IDS_COL6);
	m_files.InsertColumn(5, IDS_COL7);
	m_files.InsertColumn(6, IDS_COL5, LVCFMT_RIGHT);
	m_files.InsertColumn(7, IDS_COL8);
	m_files.UpdateColumnWidths();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	AddAnchor(IDC_EDIT1, MIDDLE_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_COMM_TXT, MIDDLE_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_PROGRESS1,BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CONTENTS, TOP_LEFT, MIDDLE_RIGHT);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestZipDlgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CResizableDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestZipDlgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

bool CTestZipDlgDlg::GetFileName(CString &szFile, bool bOpen)
{
	CFileDialog fd(bOpen, _T("zip"), bOpen ? NULL : _T("archive"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Zip Files (*.zip)|*.zip|All Files (*.*)|*.*||"));
	if (fd.DoModal() != IDOK)
		return false;
	szFile = fd.GetPathName();
	return true;
}

void CTestZipDlgDlg::Redisplay()
{
	DisplayLabel();
	m_files.DeleteAllItems();
	int iCount = m_zip.GetNoEntries();
	m_progress.Init(iCount);	
	for (int i = 0; i < iCount; i++)
	{
		CZipFileHeader fh;
		m_zip.GetFileInfo(fh, (WORD)i);
		int iItem = m_files.GetItemCount();
		CString sz = (LPCTSTR)fh.GetFileName();
		if (fh.IsEncrypted())
			sz.Insert(0, _T("(*) "));
		m_files.InsertItem(iItem, sz);
		m_files.SetItemText(iItem, 1, m_zip.IsFileDirectory((WORD)i) ? _T("YES") : _T("NO"));
		sz.Format(_T("%d"), fh.m_uUncomprSize);
		m_files.SetItemText(iItem, 2, sz);
		CTime t(fh.GetTime());
		m_files.SetItemText(iItem, 3, t.Format(_T("%d %b %Y, %X")));
		DWORD uAttr = fh.m_uExternalAttr;
		sz = uAttr & FILE_ATTRIBUTE_READONLY ? _T('r') : _T('-');
		sz += uAttr & FILE_ATTRIBUTE_ARCHIVE ? _T('a') : _T('-');
		sz += uAttr & FILE_ATTRIBUTE_HIDDEN ? _T('h') : _T('-');
		sz += uAttr & FILE_ATTRIBUTE_SYSTEM ? _T('s') : _T('-');
		m_files.SetItemText(iItem, 4, sz);
		sz.Format(_T("%d%%"), fh.m_uUncomprSize ? fh.m_uComprSize * 100 / fh.m_uUncomprSize : 0);
		m_files.SetItemText(iItem, 5, sz);
		sz.Format(_T("%lu"), fh.m_uCrc32);
		m_files.SetItemText(iItem, 6, sz);
		CString szComment = (LPCTSTR)fh.GetComment();
		sz = (szComment.GetLength() > 1024) ? szComment.Left(1024) +_T("...") : szComment;
		m_files.SetItemText(iItem, 7, sz);
		m_progress.SetPos(i + 1);
		m_progress.RedrawWindow();

	}
	m_files.UpdateColumnWidths();
	m_progress.Hide();
}

void CTestZipDlgDlg::OnDestroy() 
{
	OnArchiveClose();
	CResizableDialog::OnDestroy();
}

bool CTestZipDlgDlg::ChangeDisk(int iNumber, int , void* pData)
{
	CTestZipDlgDlg* p = (CTestZipDlgDlg*)pData;
	p->DisplayLabel();
	CString sz;
	sz.Format(_T("Insert the disk number %d."), iNumber);
	return AfxMessageBox(sz, MB_OKCANCEL) == IDOK;
}

void CTestZipDlgDlg::DisplayLabel()
{
	SetWindowText(m_zip.IsClosed(false) ? _T("Zip") : (LPCTSTR)m_zip.GetArchivePath());
}


void CTestZipDlgDlg::AddFolder(CString szFolder, CAddInfoList& list, CProgressInfo& p)
{
	szFolder.TrimRight(_T("\\"));
	CFileFind ff;
	BOOL b = ff.FindFile(szFolder + _T("\\*"));
	CStringArray folders;
	while (b)
	{
		b = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
		{
			if  (!m_options.m_bRecurse)
				continue;
			folders.Add(ff.GetFilePath());
		}
		else
		{
			CAddInfo ai;
			ai.m_szName = ff.GetFilePath();
			CFileStatus fs;
			if (CFile::GetStatus(ai.m_szName, fs))
			{
				ai.m_bDir = false;
				ai.m_iSize = fs.m_size;
				list.AddTail(ai);
				p.m_iTotal += fs.m_size;
			}
		}

	}
	for (int i = 0; i < folders.GetSize(); i++)
	{
		list.AddTail(CAddInfo(folders[i], 0, true));
												// add the folder before adding its files
												// it is not needed to add the root folder
		AddFolder(folders[i], list, p);
	}
}

void CTestZipDlgDlg::OnActionAddFiles()
{
	CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All Files (*.*)|*.*||"));
	fd.m_ofn.Flags |= OFN_ALLOWMULTISELECT;
	if (fd.DoModal() != IDOK)
		return;

	m_zip.SetPassword(m_options.m_szPassword);
	POSITION pp = fd.GetStartPosition();
	CAddInfoList list;
	CProgressInfo p;
	p.m_pProgress = &m_progress;
	while (pp)
	{
		CString sz = fd.GetNextPathName(pp);
		CFileStatus fs;
		if (CFile::GetStatus(sz, fs))
		{
			list.AddTail(CAddInfo(sz, fs.m_size));
			p.m_iTotal += fs.m_size;
		}
	}
#ifdef ZIP_ENABLE_TIMING
	UINT t = GetTickCount();
#else
	m_progress.Init(p.m_iTotal);
#endif


	try
	{
		pp = list.GetHeadPosition();
		while (pp)
		{
			CAddInfo* padi = &list.GetNext(pp);
			m_zip.AddNewFile(padi->m_szName, m_options.m_iLevel, true
#ifndef ZIP_ENABLE_TIMING
				, Callback, &p
#endif
				);
			p.m_iTotalSoFar += padi->m_iSize;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		m_bAfterException = true;
	}
	catch(...) // thrown in the STL version
	{
		m_bAfterException = true;
	}
	if (m_bAfterException)
	{
		AfxMessageBox(_T("Add files failed.  Closing the archive."), MB_ICONSTOP);
		OnArchiveClose();
	}
#ifdef ZIP_ENABLE_TIMING
	t = GetTickCount() - t;
	CString s;
	s.Format(_T("Time: %u ms"), t);
	AfxMessageBox(s);
#endif

	p.m_pProgress->Hide();	
	Redisplay();

}

// #define ZIP_ENABLE_TIMING

void CTestZipDlgDlg::OnActionAddFolder() 
{
	CBrowseForFolder bf;	
	bf.hWndOwner = this->m_hWnd;
	bf.strTitle = _T("Select directory to add to the archive");
	CString sz;
	if (!bf.GetFolder(sz))
		return;
	m_zip.SetPassword(m_options.m_szPassword);
	CProgressInfo p;
	p.m_pProgress = &m_progress;
	CAddInfoList list;
	AddFolder(sz, list, p);
#ifdef ZIP_ENABLE_TIMING
	UINT t = GetTickCount();
#else
	m_progress.Init(p.m_iTotal);
#endif

	try
	{
		POSITION pp = list.GetHeadPosition();
		while (pp)
		{
			CAddInfo* padi = &list.GetNext(pp);
			m_zip.AddNewFile(padi->m_szName, padi->m_bDir ? 0 : m_options.m_iLevel,
				true
#ifndef ZIP_ENABLE_TIMING
				, Callback, &p
#endif
				);
			p.m_iTotalSoFar += padi->m_iSize;
			
		}
	}
	catch (CException* e)
	{
		e->Delete();
		m_bAfterException = true;
	}
	catch(...) // thrown in the STL version
	{
		m_bAfterException = true;		
	}
	if (m_bAfterException)
	{
		AfxMessageBox(_T("Add folder failed.  Closing the archive."), MB_ICONSTOP);
		OnArchiveClose();
	}
	m_progress.Hide();
#ifdef ZIP_ENABLE_TIMING
	t = GetTickCount() - t;
	CString s;
	s.Format(_T("Time: %u ms"), t);
	AfxMessageBox(s);
#endif
	Redisplay();	
	
}



void CTestZipDlgDlg::OnActionDelete() 
{
	CDWordArray da;
	m_files.BuildSelectedArray(da);

	CWordArray  wa;


	for (int i = 0; i < da.GetSize(); i++)
#ifdef ZIP_NO_MFC
		wa.push_back((WORD)da[i]);
#else
		wa.Add((WORD)da[i]);
#endif

	try
	{
		m_zip.DeleteFiles(wa);
	}
	catch (CException* e)
	{
		e->Delete();
		m_bAfterException = true;
		
	}
	catch(...) // thrown in the STL version
	{
		m_bAfterException = true;		
	}
	if (m_bAfterException)
	{
		AfxMessageBox(_T("Delete failed. Closing the archive."), MB_ICONSTOP);
		OnArchiveClose();
	}

	Redisplay();	
	
}

void CTestZipDlgDlg::OnActionExtract() 
{
	CBrowseForFolder bf;	
	bf.hWndOwner = this->m_hWnd;
	bf.strTitle = _T("Select directory to extract files");
	CString sz;
	if (!bf.GetFolder(sz))
		return;
	CDWordArray da, ds;
	m_files.BuildSelectedArray(da);
	CProgressInfo p;
	p.m_pProgress = &m_progress;
	
	for (int i = 0; i < da.GetSize(); i++)
	{
		CZipFileHeader fh;
		m_zip.GetFileInfo(fh, (WORD)da[i]);
		p.m_iTotal += fh.m_uUncomprSize;
		ds.Add(fh.m_uUncomprSize);
	}
	m_progress.Init(p.m_iTotal);

	m_zip.SetPassword(m_options.m_szPassword);
	bool bErr = false;
	try
	{	
		for (int i = 0; i < da.GetSize(); i++)
		{
			m_zip.ExtractFile((WORD)da[i], sz, true, NULL, Callback, (void*) &p);
			p.m_iTotalSoFar += ds[i];
		}
	}
	catch (CException* e)
	{
		e->Delete();
		bErr = true;

	}
	catch(...) // thrown in the STL version
	{
		bErr = true;
	}
	if (bErr)
	{
		m_zip.CloseFile(NULL, true);
		AfxMessageBox(_T("Extract failed"), MB_ICONSTOP);
	}
	m_progress.Hide();	
}

void CTestZipDlgDlg::OnArchiveClose() 
{
	if (m_bCommentModified)
	{
		UpdateData();
		m_zip.SetGlobalComment(m_szComment);
		m_bCommentModified = false;
	}
	bool berr = false;
	try
	{
		m_zip.Close(m_bAfterException);	
	}
	catch (CException* e)
	{
		e->Delete();
		berr = true;
	}
	catch(...) // thrown in the STL version
	{
		berr = true;		
	}
	if (berr)
		AfxMessageBox(_T("Close failed"), MB_ICONSTOP);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	m_szComment.Empty();
	UpdateData(FALSE);
	Redisplay();
	if (m_bAfterException)
		m_bAfterException = false;
}

void CTestZipDlgDlg::OnArchiveExit() 
{
	EndDialog(IDOK);
	
}

void CTestZipDlgDlg::OnArchiveNew() 
{
	CString sz;
	if (!GetFileName(sz, false))
		return;
	OnArchiveClose();

	m_bCommentModified = false;
	m_szComment.Empty();
	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT1)->EnableWindow();
	try
	{
		m_zip.Open(sz, m_options.m_iSpan == 0 ? CZipArchive::create : CZipArchive::createSpan,
			m_options.m_iSpan == 1 ? 0 : m_options.m_uVolumeSize);
	}
	catch (CException* e)
	{
		e->Delete();
		m_bAfterException = true;
	}
	catch(...) // thrown in the STL version
	{
		m_bAfterException = true;
	}
	if (m_bAfterException)
	{
		AfxMessageBox(_T("Create new failed"), MB_ICONSTOP);
		OnArchiveClose();
	}
	Redisplay();
	
}

void CTestZipDlgDlg::OnArchiveOpen() 
{
	CString sz;
	if (!GetFileName(sz, true))
		return;
	OnArchiveClose();
	int berr = 0;
	do 
	{
		try
		{
			m_zip.Open(sz, CZipArchive::open, m_options.m_bTdComp ? 1 : 0 );
			berr = 0;
		}
		catch (CZipException* e)
		{
			if (e->m_iCause == CZipException::cdirNotFound)
				berr = -1;
			else
				berr = 1;
			e->Delete();
		}
		catch (CException* e)
		{
			e->Delete();
			berr = 1;
			
		}
		// thrown in the STL version
		catch (CZipException e)
		{
			if (e.m_iCause == CZipException::cdirNotFound)
				berr = -1;
			else
				berr = 1;

		}
		catch(...) 
		{
			berr = 1;
		}
		if (berr == -1)
		{
			if (AfxMessageBox(_T("The central directory was not found. If you're opening a multi-disk archive, make sure you have inserted the last disk. Retry?"), MB_ICONSTOP|MB_YESNO) == IDNO)
				berr = 1;
			else
				m_zip.Close(true);
		}

		if (berr == 1)
		{
			AfxMessageBox(_T("Open failed"), MB_ICONSTOP);
			return;
		}

	} while (berr == -1);
	m_bCommentModified = false;
	m_szComment = m_zip.GetGlobalComment();
	UpdateData(FALSE);
	if (!m_zip.GetSpanMode())
		GetDlgItem(IDC_EDIT1)->EnableWindow();
	Redisplay();

	
}

void CTestZipDlgDlg::OnArchiveOptions() 
{
	COptionsDlg dlg;
	dlg.m_pOptions = &m_options;
	if (dlg.DoModal() == IDOK)
	{
		if (!m_options.m_szPassword.IsEmpty())
		{
			if (!m_zip.SetPassword(m_options.m_szPassword))
			{
				if (!m_zip.IsClosed(false))
					AfxMessageBox(IDS_STRING1, MB_ICONWARNING);
				else			
				{
					m_options.m_szPassword.Empty();
					AfxMessageBox(IDS_STRING2, MB_ICONWARNING);
				}
				
			}
		}
	}
	
}

void CTestZipDlgDlg::OnHelpAbout() 
{
	
	CAboutDlg dlg;
	dlg.DoModal();
	
}

void CTestZipDlgDlg::OnUpdateActionAdd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_zip.IsClosed() && (m_zip.GetSpanMode() >= 0));
}

void CTestZipDlgDlg::OnUpdateActionDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_zip.IsClosed() && !m_zip.GetSpanMode() && m_files.GetSelectedCount());
	
}

void CTestZipDlgDlg::OnUpdateActionExtract(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_zip.IsClosed() && (m_zip.GetSpanMode() <= 0)&& m_files.GetSelectedCount());
	
}

void CTestZipDlgDlg::OnUpdateArchiveClose(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_zip.IsClosed());
	
}

void CTestZipDlgDlg::OnChangeEdit1() 
{
	m_bCommentModified = true;	
}

void CTestZipDlgDlg::OnDblclkContents(NMHDR* , LRESULT* pResult) 
{
	if (m_zip.IsClosed() || (m_zip.GetSpanMode() < 0))
		return;
	int i = m_files.GetFirstSelectedItem();
	if (i == -1)
		return;
	CCommentDlg dlg;
	CZipFileHeader fh;
	m_zip.GetFileInfo(fh, (WORD)i);
	dlg.m_szComment = fh.GetComment();
	if (dlg.DoModal() == IDOK)
	{
		if (fh.GetComment().Collate(dlg.m_szComment))
		{
			m_zip.SetFileComment((WORD)i, dlg.m_szComment);
			Redisplay();
		}
	}

	*pResult = 0;
}

void CTestZipDlgDlg::UpdateMenu(CMenu *pMenu)
{
	CCmdUI cmd;
	cmd.m_nIndexMax = pMenu->GetMenuItemCount();
	cmd.m_pMenu = pMenu;
	int s = 0;
	for (UINT i = 0; i < cmd.m_nIndexMax; i++)
	{
		cmd.m_nIndex = i;
		MENUITEMINFO mi;
		mi.cbSize = sizeof(MENUITEMINFO);
		mi.fMask = MIIM_SUBMENU
#ifndef NO_CHECK_FOR_SEPARATOR
	#if(WINVER >= 0x0500)
				| MIIM_FTYPE
	#else
				| MIIM_TYPE
	#endif /* WINVER >= 0x0500 */
#endif //NO_CHECK_FOR_SEPARATOR
;
		if (!pMenu->GetMenuItemInfo(cmd.m_nIndex, &mi, TRUE))
			return;
#ifndef NO_CHECK_FOR_SEPARATOR
		if (mi.fType & MFT_SEPARATOR)
			continue;
#endif //NO_CHECK_FOR_SEPARATOR
		if (mi.hSubMenu)
			UpdateMenu(pMenu->GetSubMenu(s++));
		else
		{
			cmd.m_nID = pMenu->GetMenuItemID(i);
			OnCmdMsg(cmd.m_nID, CN_UPDATE_COMMAND_UI, &cmd, NULL);
		}
		
	}

}

void CTestZipDlgDlg::OnInitMenu(CMenu* pMenu) 
{
	CResizableDialog::OnInitMenu(pMenu);
	UpdateMenu(pMenu);	
}

void CTestZipDlgDlg::OnActionTest() 
{
	CDWordArray da;
	m_files.BuildSelectedArray(da);
	if (da.GetSize() == 0)
		return;
	m_zip.SetPassword(m_options.m_szPassword);
	CString szReport;
	CProgressInfo p;
	p.m_pProgress = &m_progress;
	
	for (int i = 0; i < da.GetSize(); i++)
	{
		CZipFileHeader fh;
		m_zip.GetFileInfo(fh, (WORD)da[i]);
		p.m_iTotal += fh.m_uUncomprSize;
	}
	m_progress.Init(p.m_iTotal);
	for (i = 0; i < da.GetSize(); i++)
	{
		WORD nIndex = (WORD)da[i];
		bool bTestOK = false;
		try
		{
			bTestOK = m_zip.TestFile(nIndex, Callback, (void*)&p);
		}
		catch(CException*e)
		{
			e->Delete();
		}
		catch(...) // thrown in the STL version
		{
			
		}
		CZipFileHeader fh;
		VERIFY(m_zip.GetFileInfo(fh, nIndex));
		p.m_iTotalSoFar += fh.m_uUncomprSize;
		szReport += fh.GetFileName();
		szReport += CString(_T("  -  ")) + (bTestOK ? _T("Passed") : _T("Failed"));
		szReport += _T("\r\n");
	}
	m_progress.Hide();
	CDlgTestReport dlg;
	dlg.m_pszData = &szReport;
	dlg.DoModal();
}


bool CTestZipDlgDlg::Callback(int , int iSoFar, void* pData)
{
	CProgressInfo* p = static_cast<CProgressInfo*>(pData);
	iSoFar += p->m_iTotalSoFar;
	//iTotal = p->m_iTotal;

	p->m_pProgress->SetPos(iSoFar);
	p->m_pProgress->RedrawWindow();
	return true;
}
