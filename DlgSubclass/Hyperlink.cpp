/////////////////////////////
// Static.cpp

// Based on code written by Martin Jansen

#include "Hyperlink.h"

CHyperlink::CHyperlink(void)
{
	m_bUrlVisited = FALSE;
	m_crVisited = RGB(128, 0, 128);
	m_crNotVisited = RGB(0, 0, 255);
	m_bClicked = FALSE;

	LOGFONT lf;
	m_hUrlFont = (HFONT)GetStockObject(SYSTEM_FONT);
	GetObject( m_hUrlFont, sizeof(LOGFONT), &lf );
	lf.lfUnderline = TRUE;
	m_hUrlFont = CreateFontIndirect(&lf); 

	// Create the cursor
	m_hCursor = ::LoadCursor(NULL, IDC_HAND); // Load Windows' hand cursor, not available on Win95
	if( !m_hCursor )    // if not available, use the standard Arrow cursor
		m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
}

CHyperlink::~CHyperlink()
{
	::DeleteObject( m_hUrlFont );
}

BOOL CHyperlink::AttachDlgItem(UINT nID, CWnd* pParent)
{
	BOOL bSuccess = CWnd::AttachDlgItem(nID, pParent);;

	LOGFONT lf;
	::DeleteObject(m_hUrlFont); // System_font
	
	m_hUrlFont = (HFONT)::SendMessage( m_hWnd, WM_GETFONT, 0, 0);
	::GetObject(m_hUrlFont, sizeof(LOGFONT), &lf);
	lf.lfUnderline = TRUE;	
	m_hUrlFont = ::CreateFontIndirect(&lf); //create new font

	return bSuccess;
}

void CHyperlink::DrawUrl(HDC hDC) 
{	
	TCHAR szUrl[ MAX_PATH + 1 ];
	RECT rc;

	// Get the url link text
	::GetWindowText( GetHwnd(), szUrl, MAX_PATH );

	COLORREF crOldText = ::SetTextColor( hDC, m_bUrlVisited?m_crVisited:m_crNotVisited);
	int nOldBkMode = ::SetBkMode( hDC, TRANSPARENT);

	HANDLE hOldFont = ::SelectObject(hDC, m_hUrlFont);
	::GetClientRect( GetHwnd(), &rc);
	::DrawText(hDC, szUrl, -1, &rc, DT_SINGLELINE | DT_NOPREFIX);
	
	::SelectObject(hDC,hOldFont);
	::SetBkMode(hDC, nOldBkMode);
	::SetTextColor(hDC, crOldText);
}

void CHyperlink::OnLButtonDown()
{
	::SetCapture(m_hWnd);
	m_bClicked = TRUE;
}

void CHyperlink::OnLButtonUp(LPARAM lParam)
{
	::ReleaseCapture();
	if( m_bClicked )
	{	
		m_bClicked = FALSE;
		POINT pt;
		RECT rc;
		pt.x = (short)LOWORD( lParam);
		pt.y = (short)HIWORD( lParam);
		::ClientToScreen(m_hWnd, &pt);
		::GetWindowRect(m_hWnd, &rc);
		
		if(PtInRect(&rc, pt)) 
			OpenUrl();
	}
}

void CHyperlink::OpenUrl()
{
	TCHAR szUrl[ MAX_PATH + 1 ];
	// Get the url link text
	::GetWindowText(GetHwnd(), szUrl, MAX_PATH);

	if( (int)(LRESULT)::ShellExecute(NULL, "open", szUrl, NULL, NULL, SW_SHOWNORMAL ) > 32)
	{
		m_bUrlVisited = TRUE;
		// redraw the window to update the color
		::InvalidateRect(GetHwnd(), NULL, FALSE);
	}
	else
		DebugWarnMsg(TEXT("ShellExecute Failed"));
}

LRESULT CHyperlink::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hwnd, &ps);

		DrawUrl(hDC);

		EndPaint(hwnd, &ps);
		return 0L;
	} 
	case WM_LBUTTONDOWN:
		OnLButtonDown();
		break;

	case WM_LBUTTONUP:
		OnLButtonUp(lParam);
		break;

	case WM_SETCURSOR:
		::SetCursor(m_hCursor);
		return 1L;

	case WM_NCHITTEST:
		return HTCLIENT;  // Claim that the mouse is in a client area
	}
	// Now hand all messages to the original Window procedure
	return CWnd::WndProc( hwnd, uMsg, wParam, lParam );
}


