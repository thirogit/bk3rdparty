// PageHyperLinks.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageHyperLinks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_HAND MAKEINTRESOURCE(32649)

#define __URL_WWW			_T("www.prof-uis.com")
#define __URL_E_MAIL		_T("info@prof-uis.com")

#define __TOOLTIP_WWW		_T("Learn more about Prof-UIS")
#define __TOOLTIP_E_MAIL	_T("Contact us if you have any questions")

/////////////////////////////////////////////////////////////////////////////
// CPageHyperLinks dialog


CPageHyperLinks::CPageHyperLinks(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageHyperLinks::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageHyperLinks)
	//}}AFX_DATA_INIT
}


void CPageHyperLinks::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageHyperLinks)
	DDX_Control(pDX, IDC_LINK_SHOW_FOCUS, m_wndLinkShowFocus);
	DDX_Control(pDX, IDC_LINK_ENABLE, m_wndLinkEnable);
	DDX_Control(pDX, IDC_LINK_CURSOR, m_wndLinkCursor);
	DDX_Control(pDX, IDC_LINK_URL, m_wndLinkURL);
	DDX_Control(pDX, IDC_LINK_CAPTION, m_wndLinkCaption);
	DDX_Control(pDX, IDC_STATIC_URL, m_wndLabelURL);
	DDX_Control(pDX, IDC_STATIC_CAPTION, m_wndLabelCaption);
	DDX_Control(pDX, IDC_STATIC_TOOLTIP, m_wndLabelTooltip);
	DDX_Control(pDX, IDC_STATIC_CURSOR, m_wndLabelCursor);
	DDX_Control(pDX, IDC_LINK_UNDERLINE, m_wndLinkUnderline);
	DDX_Control(pDX, IDC_LINK_TOOLTIP, m_wndLinkTooltip);
	DDX_Control(pDX, IDC_LINK_COLOR_VISITED, m_wndLinkColorVisited);
	DDX_Control(pDX, IDC_LINK_COLOR_HOVER, m_wndLinkColorHover);
	DDX_Control(pDX, IDC_LINK_COLOR, m_wndLinkColor);
	DDX_Control(pDX, IDC_HYPER_LINK2, m_wndHyperLink2);
	DDX_Control(pDX, IDC_HYPER_LINK1, m_wndHyperLink1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageHyperLinks, CPageBase)
	//{{AFX_MSG_MAP(CPageHyperLinks)
	ON_CBN_SELENDOK(IDC_LINK_CURSOR, OnSelendokLinkCursor)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageHyperLinks message handlers

BOOL CPageHyperLinks::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
	m_wndLinkColor.m_lParamCookie = LPARAM(IDC_LINK_COLOR);
	m_wndLinkColorHover.m_lParamCookie = LPARAM(IDC_LINK_COLOR_HOVER);
	m_wndLinkColorVisited.m_lParamCookie = LPARAM(IDC_LINK_COLOR_VISITED);

	m_wndLinkColorVisited.m_clrDefault = 
		m_wndLinkColorVisited.m_clrSelected = m_wndHyperLink1.GetTextColorVisited();
	m_wndLinkColorHover.m_clrDefault = 
		m_wndLinkColorHover.m_clrSelected = m_wndHyperLink1.GetTextColorHover();
	m_wndLinkColor.m_clrDefault = 
		m_wndLinkColor.m_clrSelected = m_wndHyperLink1.GetTextColorNormal();

	m_wndLinkColorVisited.SetDrawFocusRect( TRUE );
	m_wndLinkColorHover.SetDrawFocusRect( TRUE );
	m_wndLinkColor.SetDrawFocusRect( TRUE );

	m_wndLinkColorVisited.SetFlat( TRUE );
	m_wndLinkColorHover.SetFlat( TRUE );
	m_wndLinkColor.SetFlat( TRUE );

	m_wndLinkUnderline.SetCheck( m_wndHyperLink1.GetTextUnderline() != false );

	int nIndex = 0;
	int nInitSel = 0;
	nIndex = m_wndLinkCursor.AddString( _T("Default") );
	m_wndLinkCursor.SetItemData( nIndex, 0 );
	nInitSel = nIndex = m_wndLinkCursor.AddString( _T("Hand") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_HAND);
	nIndex = m_wndLinkCursor.AddString( _T("Cross") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_CROSS);
	nIndex = m_wndLinkCursor.AddString( _T("UpArrow") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_UPARROW);
	nIndex = m_wndLinkCursor.AddString( _T("Wait") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_WAIT);
	nIndex = m_wndLinkCursor.AddString( _T("SizeAll") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_SIZEALL);
	m_wndLinkCursor.SetCurSel( nInitSel );

	CheckDlgButton( IDC_LINK_ENABLE, BST_CHECKED );

	m_wndHyperLink1.m_strURL = _T("http://");
	m_wndHyperLink1.m_strURL += __URL_WWW;
	m_wndHyperLink1.SetTooltipText( m_wndHyperLink1.m_strURL );
	m_wndLinkCaption.SetWindowText( __TOOLTIP_WWW );
	m_wndLinkURL.SetWindowText( m_wndHyperLink1.m_strURL );
	m_wndLinkTooltip.SetWindowText( __TOOLTIP_WWW );

	m_wndHyperLink2.m_strURL = _T("mailto:");
	m_wndHyperLink2.m_strURL += __URL_E_MAIL;
	m_wndHyperLink2.SetTooltipText( __TOOLTIP_E_MAIL );
	
	m_bInitComplete = true;

	OnCmdMsg( IDC_LINK_ENABLE, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_LINK_SHOW_FOCUS, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_LINK_UNDERLINE, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_LINK_TOOLTIP, EN_CHANGE, NULL, NULL );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CPageHyperLinks::OnCmdMsg(UINT nID, INT nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( !m_bInitComplete )
		return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	if( nCode == EN_CHANGE )
	{
		CString sText;
		switch( nID )
		{
		case IDC_LINK_TOOLTIP:
			m_wndLinkTooltip.GetWindowText( sText );
			m_wndHyperLink1.SetTooltipText( LPCTSTR(sText) );
			break;
		case IDC_LINK_CAPTION:
			m_wndLinkCaption.GetWindowText( sText );
			m_wndHyperLink1.SetWindowText( LPCTSTR(sText) );
			break;
		case IDC_LINK_URL:
			m_wndLinkURL.GetWindowText( sText );
			m_wndHyperLink1.m_strURL = LPCTSTR(sText);
			break;
		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		return TRUE;
	}

	if( nCode == CN_COMMAND )
	{
		BOOL bChecked = FALSE;
		
		switch( nID )
		{
		case IDC_LINK_ENABLE:
			bChecked = IsDlgButtonChecked( nID );
			m_wndHyperLink1.EnableWindow( bChecked );
			m_wndHyperLink2.EnableWindow( bChecked );
			break;
		
		case IDC_LINK_SHOW_FOCUS:
			bChecked = IsDlgButtonChecked( nID );
			m_wndHyperLink1.SetDrawFocusRect( bChecked );
			m_wndHyperLink2.SetDrawFocusRect( bChecked );
			break;

		case IDC_LINK_UNDERLINE:
			bChecked = IsDlgButtonChecked( nID );
			m_wndHyperLink1.SetTextUnderline( bChecked != FALSE );
			m_wndHyperLink2.SetTextUnderline( bChecked != FALSE );
			break;

		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		return TRUE;
	}
	return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//////////////////////////////////////////////////////////////////////////

LRESULT CPageHyperLinks::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	COLORREF clrColor = COLORREF(wParam);

	switch( lParam )
	{
	case IDC_LINK_COLOR:
		m_wndHyperLink1.SetTextColorNormal( clrColor );
		m_wndHyperLink2.SetTextColorNormal( clrColor );
		break;
	case IDC_LINK_COLOR_HOVER:
		m_wndHyperLink1.SetTextColorHover( clrColor );
		m_wndHyperLink2.SetTextColorHover( clrColor );
		break;
	case IDC_LINK_COLOR_VISITED:
		m_wndHyperLink1.SetTextColorVisited( clrColor );
		m_wndHyperLink2.SetTextColorVisited( clrColor );
		break;
	default:
		break;
	} // switch( lParam )
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CPageHyperLinks::OnSelendokLinkCursor() 
{
int nCurSel = m_wndLinkCursor.GetCurSel();
int nCursorType = int( m_wndLinkCursor.GetItemData( nCurSel ) );
	if( nCursorType == 0 )
		nCursorType = -1;
	m_wndHyperLink1.SetBtnCursor( nCursorType );
	m_wndHyperLink2.SetBtnCursor( nCursorType );
}
