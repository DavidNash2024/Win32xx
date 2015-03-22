/////////////////////////////
// Hyperlink.cpp

#include "stdafx.h"
#include "Hyperlink.h"

CHyperlink::CHyperlink() : m_IsUrlVisited(FALSE), m_IsClicked(FALSE), m_crVisited(RGB(128, 0, 128)),
                            m_crNotVisited(RGB(0,0,255))
{
	// Create the cursor
	m_hCursor = ::LoadCursor(NULL, IDC_HAND);

	// IDC_HAND is not available on Win95, so load a reasonable alternative
	if( !m_hCursor )
		m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
}

CHyperlink::~CHyperlink()
{
}

void CHyperlink::OnAttach()
{
	SetWindowText(_T("Win32++"));
	NONCLIENTMETRICS ncm;
	ncm.cbSize = GetSizeofNonClientMetrics();
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
	LOGFONT lf = ncm.lfMessageFont;
	lf.lfUnderline = TRUE;
	m_UrlFont.CreateFontIndirect(&lf);
}

void CHyperlink::OnLButtonDown()
{
	SetCapture();
	m_IsClicked = TRUE;
}

void CHyperlink::OnLButtonUp(LPARAM lParam)
{
	ReleaseCapture();
	if(m_IsClicked)
	{
		m_IsClicked = FALSE;
		CPoint pt;
		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);
		ClientToScreen(pt);
		CRect rc = GetWindowRect(); 

		if (rc.PtInRect(pt)) 
			OpenUrl();
	}
}

void CHyperlink::OpenUrl()
{
	TCHAR szUrl[ MAX_PATH + 1 ] = _T("http://sourceforge.net/projects/win32-framework/");

	if( (int)(LRESULT)::ShellExecute(NULL, _T("open"), szUrl, NULL, NULL, SW_SHOWNORMAL ) > 32)
	{
		m_IsUrlVisited = TRUE;

		// redraw the StatusBar to update the color
		GetParent().RedrawWindow();
	}
}

LRESULT CHyperlink::OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	// Messages such as WM_CTLCOLORSTATIC are reflected back to the CWnd object that created them.
	if (uMsg ==  WM_CTLCOLORSTATIC)
	{
		CDC* pDC = CDC::FromHandle((HDC)wParam);
		pDC->SetTextColor( m_IsUrlVisited? m_crVisited : m_crNotVisited);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(&m_UrlFont);

		// Return a NULL brush for a transparent background
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}
	return 0L;
}

LRESULT CHyperlink::OnSetCursor()
{
	// Must use ::SetCursor here. CStatic::SetCursor does not do the same thing. 
	::SetCursor(m_hCursor);
	
	return 1L;	// Non-zero return prevents default processing
}

LRESULT CHyperlink::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:  OnLButtonDown();		break;
	case WM_LBUTTONUP:	  OnLButtonUp(lParam);	break;
	case WM_SETCURSOR:	  return OnSetCursor();	
	case WM_NCHITTEST:	  return HTCLIENT;		// Claim that the mouse is in a client area
	}

	return WndProcDefault(uMsg, wParam, lParam);
}


