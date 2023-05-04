// This is part of the Professional User Interface Suite library.
// Copyright (C) 2001-2009 FOSS Software, Inc.
// All rights reserved.
//
// http://www.prof-uis.com
// mailto:support@prof-uis.com
//
// This source code can be used, modified and redistributed
// under the terms of the license agreement that is included
// in the Professional User Interface Suite package.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL FOSS SOFTWARE INC. BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#include "stdafx.h"

#if (!defined __EXT_LABEL_H)
	#include <ExtLabel.h>
#endif

#if (!defined __EXT_PAINT_MANAGER_H)
	#include <ExtPaintManager.h>
#endif

#if (!defined __EXT_POPUP_MENU_WND_H)
	#include <ExtPopupMenuWnd.h>
#endif

#if (!defined __EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtLabel

IMPLEMENT_DYNCREATE( CExtLabel, CStatic );
IMPLEMENT_CExtPmBridge_MEMBERS( CExtLabel );

bool CExtLabel::g_bHelper_AutoAdjust_SS_NOTIFY_style = true;

CExtLabel::CExtLabel()
	: m_bFontBold(false)
	, m_bFontItalic(false)
	, m_bFontUnderline(false)
	, m_bFontStrikeOut(false)
	, m_clrTextNormal( COLORREF(-1L) )
	, m_clrTextDisabled( COLORREF(-1L) )
	, m_clrBackground( COLORREF(-1L) )
	, m_bInitText(false)
	, m_sText( _T("") )
	, m_eImageMode( eAlign )
	, m_nAdvancedTipStyle( INT(CExtPopupMenuTipWnd::__ETS_RECTANGLE_NO_ICON) )
	, m_bHelper_AutoAdjust_SS_NOTIFY_style(  CExtLabel::g_bHelper_AutoAdjust_SS_NOTIFY_style )
	, m_rcLabelTextMargins( 0, 0, 0, 0 )
{
	m_bmp.Empty();
	PmBridge_Install();
}

CExtLabel::~CExtLabel()
{
	m_bmp.Empty();
	PmBridge_Uninstall();
}

BEGIN_MESSAGE_MAP(CExtLabel, CStatic)
	//{{AFX_MSG_MAP(CExtLabel)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CExtLabel::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}

void CExtLabel::DoPaint(
	CDC * pDC,
	CRect & rcClient
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pDC );
CExtMemoryDC dc( pDC, &rcClient );
CRgn rgnClient;
	if( rgnClient.CreateRectRgnIndirect( &rcClient ) )
		dc.SelectClipRgn( &rgnClient );
	OnEraseBackground( dc, rcClient );
DWORD dwWndStyle = GetStyle();
DWORD dwWndType = (dwWndStyle&SS_TYPEMASK);
bool bCenterImage = ( (dwWndStyle&SS_CENTERIMAGE) != 0 );
	if( ! m_bmp.IsEmpty() )
	{
		bool bSmootherAsPossible = true;
		e_ImageMode_t eImageMode = GetImageMode();
		if( eImageMode == eStretch )
			m_bmp.AlphaBlendSkinParts( dc.GetSafeHdc(), rcClient, CRect(0,0,0,0), CExtBitmap::__EDM_STRETCH, true, bSmootherAsPossible );
		else if( eImageMode == eTouchInside || eImageMode == eTouchOutside )
		{
			CRect rcTouchSurface = rcClient;
			CExtMemoryDC dcTouch( &dc, rcTouchSurface, CExtMemoryDC::MDCOPT_TO_MEMORY|CExtMemoryDC::MDCOPT_FILL_BITS|CExtMemoryDC::MDCOPT_RTL_COMPATIBILITY );
			if( dcTouch.GetSafeHdc() )
			{
				CSize sizeTouchSrc = rcTouchSurface.Size();
				CSize sizeBmp = m_bmp.GetSize();
				double lfAspectX = double(rcTouchSurface.Width())  / double(sizeBmp.cx);
				double lfAspectY = double(rcTouchSurface.Height()) / double(sizeBmp.cy);
				double lfAspect = ( eImageMode == eTouchInside ) ? ( min( lfAspectX, lfAspectY ) ) : ( max( lfAspectX, lfAspectY ) );
				CSize sizeNew( LONG(double(sizeBmp.cx)*lfAspect), LONG(double(sizeBmp.cy)*lfAspect) );
				CRect rcTouchDst( rcTouchSurface.left, rcTouchSurface.top, rcTouchSurface.left + sizeNew.cx, rcTouchSurface.top + sizeNew.cy );
				if( eImageMode == eTouchInside )
				{				
					if( sizeNew.cx > sizeTouchSrc.cx )
						rcTouchDst.OffsetRect( ( sizeNew.cx - sizeTouchSrc.cx ) / 2, 0 );
					if( sizeNew.cy > sizeTouchSrc.cy )
						rcTouchDst.OffsetRect( 0, ( sizeNew.cy - sizeTouchSrc.cy ) / 2 );
				}
				else
				{
					rcTouchDst.OffsetRect( - ( sizeNew.cx - sizeTouchSrc.cx ) / 2, 0 );
					rcTouchDst.OffsetRect( 0, - ( sizeNew.cy - sizeTouchSrc.cy ) / 2 );
				}
				INT nOldStretchBltMode = bSmootherAsPossible ? ( ::GetStretchBltMode( dcTouch.m_hDC ) ) : ( COLORONCOLOR ) ;
				if( bSmootherAsPossible )
					::SetStretchBltMode( dcTouch.m_hDC, ( g_PaintManager.m_bIsWinNT ) ? HALFTONE : COLORONCOLOR );
				m_bmp.AlphaBlend( dcTouch.m_hDC, rcTouchDst );
				if( bSmootherAsPossible )
					::SetStretchBltMode( dcTouch.m_hDC, nOldStretchBltMode );
			}
		}
		else if( eImageMode == eTile )
			m_bmp.AlphaBlendSkinParts(  dc.GetSafeHdc(), rcClient, CRect(0,0,0,0), CExtBitmap::__EDM_TILE, true, bSmootherAsPossible );
		else if( eImageMode == eAlign )
		{
			CSize szSize = m_bmp.GetSize();
			CRect rcDst( rcClient.left, rcClient.top, rcClient.left + szSize.cx, rcClient.top + szSize.cy );
			bool bCenterHorizontally = false;
			switch( dwWndType )
			{
			case SS_RIGHT:  rcDst.OffsetRect( rcClient.right - rcDst.right, 0 ); break;
			case SS_CENTER: bCenterHorizontally = true;        break;
			default: /* all the other types assumed as left */ break;
			}
			if( bCenterHorizontally )
				rcDst.OffsetRect( ( (rcClient.right - rcClient.left) - (rcDst.right - rcDst.left) ) / 2, 0 );
			if( bCenterImage )
				rcDst.OffsetRect( 0, ( (rcClient.bottom - rcClient.top) - (rcDst.bottom - rcDst.top) ) / 2 );
			CRect rcSrc( 0, 0, szSize.cx, szSize.cy );
			rcDst.top = max( rcDst.top, rcClient.top );
			rcDst.left = max( rcDst.left, rcClient.left );
			rcDst.bottom = min( rcDst.bottom, rcClient.bottom );
			rcDst.right = min( rcDst.right, rcClient.right );
			if( ::RectVisible( dc.GetSafeHdc(), &rcDst ) )
			{
				INT nOldStretchBltMode = bSmootherAsPossible ? ( ::GetStretchBltMode( dc.m_hDC ) ) : ( COLORONCOLOR );
				if( bSmootherAsPossible )
					::SetStretchBltMode( dc.m_hDC, ( g_PaintManager.m_bIsWinNT ) ? HALFTONE : COLORONCOLOR );
				m_bmp.AlphaBlend( dc.m_hDC, rcDst, rcSrc );
				if( bSmootherAsPossible )
					::SetStretchBltMode( dc.m_hDC, nOldStretchBltMode );
			}
		}
	}
	else if( dwWndType == SS_ICON )
	{
		HICON hIcon = GetIcon();
		if( hIcon != NULL )
		{
			CExtCmdIcon _icon;
			_icon.AssignFromHICON( hIcon, true );
			CSize szIcon = _icon.GetSize();
			int nOffsetX = bCenterImage ? (rcClient.Width() - szIcon.cx) / 2 : 0;
			int nOffsetY = bCenterImage ? (rcClient.Height() - szIcon.cy) / 2 : 0;
			_icon.Paint(
				PmBridge_GetPM(), dc.GetSafeHdc(), rcClient.left + nOffsetX, rcClient.top + nOffsetY, -1, -1,
				IsWindowEnabled() ? CExtCmdIcon::__PAINT_NORMAL : CExtCmdIcon::__PAINT_DISABLED
				);
		}
	}
	else
	{
		CExtSafeString strText;
		int nTextLen = GetWindowTextLength();
		if( nTextLen > 0 )
		{
			GetWindowText( strText.GetBuffer( nTextLen + 2 ), nTextLen + 1 );
			strText.ReleaseBuffer();
		}
		if( strText.GetLength() > 0 )
		{
			DWORD dwDrawTextFlags = 0;
			switch( dwWndType )
			{
			case SS_RIGHT:           dwDrawTextFlags = DT_RIGHT;  break; 
			case SS_CENTER:          dwDrawTextFlags = DT_CENTER; break;
			case SS_LEFTNOWORDWRAP:  dwDrawTextFlags = DT_LEFT;   break;
			default: /* all the other types assumed as left */ dwDrawTextFlags = DT_LEFT; break;
			} // switch( dwWndType )
			if( strText.Find( _T('\t') ) != -1 ) // do tabs expanding
				dwDrawTextFlags |= DT_EXPANDTABS;
			if(		(dwWndType == SS_SIMPLE)
				||	(dwWndStyle&(SS_CENTERIMAGE|SS_ENDELLIPSIS|SS_PATHELLIPSIS)) != 0
				)
			{
				dwDrawTextFlags |= DT_SINGLELINE;
				if( (dwWndStyle&SS_CENTERIMAGE) != 0 )
					dwDrawTextFlags |= DT_VCENTER;
				if( (dwWndStyle&SS_ENDELLIPSIS) != 0 )
					dwDrawTextFlags |= DT_END_ELLIPSIS;
				if( (dwWndStyle&SS_PATHELLIPSIS) != 0 )
					dwDrawTextFlags |= DT_PATH_ELLIPSIS;
			}
			else
				dwDrawTextFlags |= DT_WORDBREAK;
			if( dwWndType == SS_LEFTNOWORDWRAP )
				dwDrawTextFlags &= ~(DT_WORDBREAK|DT_SINGLELINE);
			if( (dwWndStyle&SS_NOPREFIX) != 0 )
				dwDrawTextFlags |= DT_NOPREFIX;
			bool bEnabled = IsWindowEnabled() ? true : false;
			OnDrawLabelText( dc, rcClient, strText, dwDrawTextFlags, bEnabled );
		} // if( strText.GetLength() > 0 )
	}
	PmBridge_GetPM()->OnPaintSessionComplete( this );
	if( rgnClient.GetSafeHandle() != NULL )
		dc.SelectClipRgn( &rgnClient );	
}

void CExtLabel::OnPaint() 
{
	ASSERT_VALID( this );
DWORD dwWndStyle = GetStyle();
DWORD dwWndType = (dwWndStyle&SS_TYPEMASK);
	if(		dwWndType == SS_BLACKRECT 
		||	dwWndType == SS_GRAYRECT 
		||	dwWndType == SS_WHITERECT 
		||	dwWndType == SS_BLACKFRAME 
		||	dwWndType == SS_GRAYFRAME 
		||	dwWndType == SS_WHITEFRAME 
		||	dwWndType == SS_USERITEM 
		||	dwWndType == SS_OWNERDRAW 
		||	dwWndType == SS_BITMAP 
		||	dwWndType == SS_ENHMETAFILE 
		||	dwWndType == SS_ETCHEDHORZ 
		||	dwWndType == SS_ETCHEDVERT 
		||	dwWndType == SS_ETCHEDFRAME 
		)
	{
		Default();
		return;
	}
CPaintDC dcPaint( this );
CRect rcClient;
	GetClientRect( &rcClient );
	if( rcClient.IsRectEmpty() )
		return;
	DoPaint( &dcPaint, rcClient );
}

void CExtLabel::OnEraseBackground(
	CDC & dc,
	const CRect & rcClient
	)
{
	ASSERT_VALID( this );
COLORREF clrBackground = GetBkColor();
bool bTransparent = false;
	if(		PmBridge_GetPM()->GetCb2DbTransparentMode(this)
		&&	( clrBackground == COLORREF(-1L) )
		)
	{
		CExtPaintManager::stat_ExcludeChildAreas(
			dc,
			GetSafeHwnd(),
			CExtPaintManager::stat_DefExcludeChildAreaCallback
			);
		if( PmBridge_GetPM()->PaintDockerBkgnd( true, dc, this ) )
			bTransparent = true;
	}
	if( ! bTransparent )
		dc.FillSolidRect(
			&rcClient,
			(clrBackground != COLORREF(-1L)) 
				? clrBackground 
				: PmBridge_GetPM()->GetColor( CExtPaintManager::CLR_3DFACE_OUT, this ) 
			);	
}

HFONT CExtLabel::OnQueryFont() const
{
	ASSERT_VALID( this );
HFONT hFont = NULL;
	if( GetSafeHwnd() != NULL )
		hFont = (HFONT) ::SendMessage( m_hWnd, WM_GETFONT, 0L, 0L );
	if( hFont == NULL )
	{
		HWND hWndParent = ::GetParent( m_hWnd );
		if( hWndParent != NULL )
			hFont = (HFONT)
				::SendMessage( hWndParent, WM_GETFONT, 0L, 0L );
	} // if( hFont == NULL )
	if( hFont == NULL )
	{
		hFont = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
		if( hFont == NULL )
			hFont = (HFONT)::GetStockObject( SYSTEM_FONT );
	} // if( hFont == NULL )
	return hFont;
}

void CExtLabel::OnDrawLabelText(
	CDC & dc,
	const RECT & rcText,
	__EXT_MFC_SAFE_LPCTSTR strText,
	DWORD dwDrawTextFlags,
	bool bEnabled
	)
{
HFONT hFont = OnQueryFont();
	ASSERT( hFont != NULL );

LOGFONT lf;
	::GetObject( 
		(HGDIOBJ) hFont, 
        sizeof( lf ), 
        (LPVOID) & lf 
		);
	hFont = NULL;
	
	if( m_bFontBold )
		lf.lfWeight = 
			(lf.lfWeight > FW_BOLD) 
				? lf.lfWeight 
				: FW_BOLD;
	lf.lfItalic = (BYTE)( m_bFontItalic ? 1 : 0 );
	lf.lfUnderline = (BYTE)( m_bFontUnderline ? 1 : 0 );
	lf.lfStrikeOut = (BYTE)( m_bFontStrikeOut ? 1 : 0 );
	
	hFont = ::CreateFontIndirect( &lf );
	ASSERT( hFont != NULL );
CFont _fontDestructor;
	_fontDestructor.Attach( hFont );
	
HGDIOBJ hOldFont = NULL;
	if( hFont != NULL )
		hOldFont = ::SelectObject( dc, (HGDIOBJ)hFont );
INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
COLORREF clrOldText =
		dc.SetTextColor( OnQueryTextColor( bEnabled ) );

CRect rc( rcText );
	rc.DeflateRect( &m_rcLabelTextMargins );
	dc.DrawText(
		LPCTSTR(strText),
		int(_tcslen(strText)),
		rc,
		dwDrawTextFlags
		);
	dc.SetTextColor( clrOldText );
	dc.SetBkMode( nOldBkMode );
	if( hFont != NULL )
		::SelectObject( dc, hOldFont );
}

COLORREF CExtLabel::OnQueryTextColor(
	bool bEnabled
	) const
{
	ASSERT_VALID( this );
COLORREF clrText = GetTextColor( bEnabled );
	if( clrText != COLORREF(-1L) )
		return clrText;
CWindowDC dcFake( NULL );
	clrText =
		PmBridge_GetPM()->QueryObjectTextColor(
			dcFake,
			bEnabled,
			false,
			false,
			false,
			(CObject*)this
			);
	if( clrText != COLORREF(-1L) )
		return clrText;
	clrText =
		PmBridge_GetPM()->GetColor(
			bEnabled
				? COLOR_BTNTEXT
				: CExtPaintManager::CLR_TEXT_DISABLED,
			(CObject*)this
			);
	return clrText;
}

LRESULT CExtLabel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
#if (defined WM_UPDATEUISTATE)	
	ASSERT( WM_UPDATEUISTATE == 0x0128 );
#endif
	// WM_UPDATEUISTATE causes repaint without WM_PAINT, so we eat it
	if( message == 0x0128 )
		return 0;
	if(	message == WM_ENABLE )
	{
		LRESULT lResult = CStatic::WindowProc(message, wParam, lParam);
		Invalidate();
		UpdateWindow();
		return lResult;
	}
	if( message == WM_PRINT || message == WM_PRINTCLIENT )
	{
		CDC * pDC = CDC::FromHandle( (HDC)wParam );
		CRect rcClient;
		GetClientRect( &rcClient );
		DoPaint( pDC, rcClient );
		HWND hWndChild = ::GetWindow( m_hWnd, GW_CHILD );
		for( ; hWndChild != NULL; hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT ) )
		{
			__EXT_MFC_LONG_PTR dwChildStyle = ::__EXT_MFC_GetWindowLong( hWndChild, GWL_STYLE );
			if( (dwChildStyle&WS_VISIBLE) == 0 )
				continue;
			CRect rcChildWnd, rcChildClient, rcStartClient;
			
			::GetClientRect( m_hWnd, &rcStartClient );
			::ClientToScreen( hWndChild, ((LPPOINT)(&rcStartClient)) );
			::ClientToScreen( hWndChild, ((LPPOINT)(&rcStartClient))+1 );

			::GetWindowRect( hWndChild, &rcChildWnd );
			::GetClientRect( hWndChild, &rcChildClient );
			::ClientToScreen( hWndChild, ((LPPOINT)(&rcChildClient)) );
			::ClientToScreen( hWndChild, ((LPPOINT)(&rcChildClient))+1 );
			CPoint ptChildRenderOffset( 0, 0 );
			if( (lParam&PRF_NONCLIENT) != 0 )
			{
				ptChildRenderOffset.x = rcStartClient.left - rcChildWnd.left;
				ptChildRenderOffset.y = rcStartClient.top - rcChildWnd.top;
			}
			else
			{
				ptChildRenderOffset.x = rcStartClient.left - rcChildClient.left;
				ptChildRenderOffset.y = rcStartClient.top - rcChildClient.top;
			}
			if(		ptChildRenderOffset.x != 0
				||	ptChildRenderOffset.y != 0
				)
				::OffsetViewportOrgEx(
					pDC->m_hDC,
					-ptChildRenderOffset.x,
					-ptChildRenderOffset.y,
					NULL
					);
			::SendMessage(
				hWndChild,
				message,
				(WPARAM)pDC->m_hDC,
				lParam
				);
//			CExtPaintManager::stat_PrintChildren(
//				hWndChild,
//				message,
//				pDC->m_hDC,
//				lParam,
//				true,
//				this,
//				lParam
//				);
			if(		ptChildRenderOffset.x != 0
				||	ptChildRenderOffset.y != 0
				)
				::OffsetViewportOrgEx(
					pDC->m_hDC,
					ptChildRenderOffset.x,
					ptChildRenderOffset.y,
					NULL
					);
		} // for( ; hWndChild != NULL; hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT ) )
		
		return (!0);
	}

	if( message == WM_MOUSEMOVE )
	{
		if(		m_nAdvancedTipStyle != INT(CExtPopupMenuTipWnd::__ETS_NONE)
			&&	( ! m_strToolTipText.IsEmpty() )
			&&	(! CExtPopupMenuWnd::IsMenuTracking() )
			)
		{
			CExtPopupMenuTipWnd * pATTW = OnAdvancedPopupMenuTipWndGet();
			if( pATTW != NULL )
			{
				CRect rcArea;
				GetWindowRect( &rcArea );
				OnAdvancedPopupMenuTipWndDisplay( *pATTW, rcArea );
			}
		}
	}
	else
	if(		message == WM_SETTEXT 
		||	message == WM_GETTEXT 
		||	message == WM_GETTEXTLENGTH 
		)
	{
		DWORD dwWndStyle = GetStyle();
		DWORD dwWndType = (dwWndStyle&SS_TYPEMASK);
		if(		dwWndType == SS_BLACKRECT 
			||	dwWndType == SS_GRAYRECT 
			||	dwWndType == SS_WHITERECT 
			||	dwWndType == SS_BLACKFRAME 
			||	dwWndType == SS_GRAYFRAME 
			||	dwWndType == SS_WHITEFRAME 
			||	dwWndType == SS_USERITEM 
			||	dwWndType == SS_OWNERDRAW 
			||	dwWndType == SS_BITMAP 
			||	dwWndType == SS_ICON
			||	dwWndType == SS_ENHMETAFILE 
			||	dwWndType == SS_ETCHEDHORZ 
			||	dwWndType == SS_ETCHEDVERT 
			||	dwWndType == SS_ETCHEDFRAME 
			)
			return CStatic::WindowProc( message, wParam, lParam );

		if(		(!m_bInitText)
			&&	(message == WM_GETTEXT || message == WM_GETTEXTLENGTH)
			)
		{
			LRESULT lResult = CStatic::WindowProc( message, wParam, lParam );
			
			INT nMaxLength = 0;
			if( message != WM_GETTEXTLENGTH )
			{
				WPARAM wParamLocal = 0L;
				LPARAM lParamLocal = 0L; 
				nMaxLength = (INT)
					CStatic::WindowProc( 
						WM_GETTEXTLENGTH, 
						wParamLocal, 
						lParamLocal 
						);
			}
			else
				nMaxLength = (INT)lResult;
			CString sTextInit;
			CStatic::WindowProc( 
				WM_GETTEXT, 
				nMaxLength + 1, 
				(LPARAM)sTextInit.GetBuffer( nMaxLength + 1 ) 
				);
			sTextInit.ReleaseBuffer();
			m_sText = sTextInit;

			m_bInitText = true;

			return lResult;
		}

		if( message == WM_SETTEXT )
		{
			LPCTSTR lpszText = (LPCTSTR)lParam;
			m_sText = lpszText;
			m_bInitText = true;
			RedrawWindow(
				NULL, NULL,
				RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW
				);
			return TRUE;
		}
		else if( message == WM_GETTEXT )
		{
			TCHAR * lpszText = (TCHAR *)lParam;
			::memset( lpszText, 0, wParam );

			__EXT_MFC_STRNCPY(
				lpszText,
				wParam,
				m_sText,
				wParam - 1
				);
			lpszText[ wParam - 1 ] = _T('\0');

			return m_sText.GetLength();
		}
		else if( message == WM_GETTEXTLENGTH )
		{
			return m_sText.GetLength();
		}
	}

	return CStatic::WindowProc(message, wParam, lParam);
}

void CExtLabel::PreSubclassWindow()
{
	m_bInitText = false;
	CStatic::PreSubclassWindow();
	if( m_bHelper_AutoAdjust_SS_NOTIFY_style )
		ModifyStyle(
			m_strToolTipText.IsEmpty() ? SS_NOTIFY : 0,
			m_strToolTipText.IsEmpty() ? 0 : SS_NOTIFY
			);
}

void CExtLabel::SetFontBold( 
	bool bSet // = true 
	)
{
	ASSERT_VALID( this );
	m_bFontBold = bSet;
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

void CExtLabel::SetFontItalic( 
	bool bSet // = true 
	)
{
	ASSERT_VALID( this );
	m_bFontItalic = bSet;
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

void CExtLabel::SetFontUnderline( 
	bool bSet // = true 
	)
{
	ASSERT_VALID( this );
	m_bFontUnderline = bSet;
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

void CExtLabel::SetFontStrikeOut( 
	bool bSet // = true 
	)
{
	ASSERT_VALID( this );
	m_bFontStrikeOut = bSet;
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

void CExtLabel::SetBkColor( COLORREF clrBk )
{ 
	ASSERT_VALID( this );
	m_clrBackground = clrBk; 
	if( GetSafeHwnd() != NULL )
		Invalidate();
}

void CExtLabel::SetTextColor(
	bool bEnabled,
	COLORREF clrText // = COLORREF(-1L)
	)
{
	ASSERT_VALID( this );
	if( bEnabled )
		m_clrTextNormal = clrText;
	else
		m_clrTextDisabled = clrText;
	if( GetSafeHwnd() != NULL )
		Invalidate();
}
	
bool CExtLabel::GetFontBold()
{
	ASSERT_VALID( this );
	return m_bFontBold;
}

bool CExtLabel::GetFontItalic()
{
	ASSERT_VALID( this );
	return m_bFontItalic;
}

bool CExtLabel::GetFontUnderline()
{
	ASSERT_VALID( this );
	return m_bFontUnderline;
}

bool CExtLabel::GetFontStrikeOut()
{
	ASSERT_VALID( this );
	return m_bFontStrikeOut;
}

COLORREF CExtLabel::GetBkColor() const
{ 
	ASSERT_VALID( this );
	return m_clrBackground; 
}

COLORREF CExtLabel::GetTextColor(
	bool bEnabled
	) const
{
	ASSERT_VALID( this );
COLORREF clrText =
		bEnabled
			? m_clrTextNormal
			: m_clrTextDisabled
			;
	return clrText;
}

const CExtBitmap * CExtLabel::GetBitmapEx() const
{
	ASSERT_VALID( this );
	return 
		( const_cast < CExtLabel * > ( this ) ) -> GetBitmapEx();
}

CExtBitmap * CExtLabel::GetBitmapEx()
{
	ASSERT_VALID( this );
	return (&m_bmp);
}

bool CExtLabel::SetBitmapEx( 
	CExtBitmap * pBmp 
	)
{
	ASSERT_VALID( this );
	if(		pBmp == NULL
		||	pBmp->IsEmpty()
		)
	{
		m_bmp.Empty();
		if( GetSafeHwnd() != NULL )
			Invalidate();
		return true;
	}
bool bRetVal = m_bmp.AssignFromOther( *pBmp );
	if( GetSafeHwnd() != NULL )
		Invalidate();
	return bRetVal;
}

CExtLabel::e_ImageMode_t CExtLabel::GetImageMode() const
{
	ASSERT_VALID( this );
	return m_eImageMode;
}

CExtLabel::e_ImageMode_t CExtLabel::SetImageMode( 
	e_ImageMode_t eImageMode 
	)
{
	ASSERT_VALID( this );
e_ImageMode_t eImageModeOld = m_eImageMode;
	m_eImageMode = eImageMode;
	return eImageModeOld;
}

void CExtLabel::SetTooltipText( int nId )
{
	ASSERT_VALID( this );
CExtSafeString sText;
	g_ResourceManager->LoadString( sText, nId );
	SetTooltipText( LPCTSTR(sText) );
}

void CExtLabel::SetTooltipText( __EXT_MFC_SAFE_LPCTSTR sText )
{
	ASSERT_VALID( this );
	if( sText == NULL )
		sText = _T("");
	m_strToolTipText = sText;
	if(		m_bHelper_AutoAdjust_SS_NOTIFY_style
		&&	GetSafeHwnd() != NULL
		)
		ModifyStyle(
			m_strToolTipText.IsEmpty() ? SS_NOTIFY : 0,
			m_strToolTipText.IsEmpty() ? 0 : SS_NOTIFY
			);
}

CExtPopupMenuTipWnd * CExtLabel::OnAdvancedPopupMenuTipWndGet() const
{
	if( m_nAdvancedTipStyle == INT(CExtPopupMenuTipWnd::__ETS_NONE) )
		return NULL;
	return (&( CExtPopupMenuSite::g_DefPopupMenuSite.GetTip() ));
}

void CExtLabel::OnAdvancedPopupMenuTipWndDisplay(
	CExtPopupMenuTipWnd & _ATTW,
	const RECT & rcExcludeArea
	) const
{
	ASSERT_VALID( this );
	if( m_strToolTipText.IsEmpty() )
		return;
	_ATTW.SetText( LPCTSTR(m_strToolTipText) );
	_ATTW.SetTipStyle( (CExtPopupMenuTipWnd::e_tip_style_t)m_nAdvancedTipStyle );
	_ATTW.Show( (CWnd*)this, rcExcludeArea );
}


