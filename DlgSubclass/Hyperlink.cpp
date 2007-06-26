/////////////////////////////
// Hyperlink.cpp

#include "Hyperlink.h"

CHyperlink::CHyperlink() : m_bUrlVisited(FALSE), m_bClicked(FALSE), m_crVisited(RGB(128, 0, 128)), 
                            m_crNotVisited(RGB(0,0,255)), m_hUrlFont(NULL)
{
	// Create the cursor
	m_hCursor = ::LoadCursor(NULL, IDC_HAND); 
	
	// IDC_HAND is not available on Win95, so load a reasonable alternative
	if( !m_hCursor )    
		m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
}

CHyperlink::~CHyperlink()
{
	if (m_hUrlFont)  ::DeleteObject(m_hUrlFont);
}

BOOL CHyperlink::AttachDlgItem(UINT nID, CWnd* pParent)
{
	BOOL bSuccess = CWnd::AttachDlgItem(nID, pParent);;

	LOGFONT lf;
	m_hUrlFont = (HFONT)::SendMessage( m_hWnd, WM_GETFONT, 0, 0);
	::GetObject(m_hUrlFont, sizeof(LOGFONT), &lf);
	lf.lfUnderline = TRUE;	
	m_hUrlFont = ::CreateFontIndirect(&lf); 

	return bSuccess;
}

void CHyperlink::OnLButtonDown()
{
	::SetCapture(m_hWnd);
	m_bClicked = TRUE;
}

void CHyperlink::OnLButtonUp(LPARAM lParam)
{
	::ReleaseCapture();
	if(m_bClicked)
	{	
		m_bClicked = FALSE;
		POINT pt;
		RECT rc;
		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);
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

LRESULT CHyperlink::OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Messages such as WM_CTLCOLORSTATIC are reflected back to the CWnd object that created them.
	if (uMsg ==  WM_CTLCOLORSTATIC)
	{ 
		HDC hDC = (HDC)wParam; 
	
		::SetTextColor(hDC, m_bUrlVisited? m_crVisited : m_crNotVisited);
		::SetBkMode(hDC, TRANSPARENT);
		::SelectObject(hDC, m_hUrlFont);
		return (LRESULT)::GetSysColorBrush(COLOR_BTNFACE);
	}
	return 0L;
}

LRESULT CHyperlink::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		OnLButtonDown();
		break;

	case WM_LBUTTONUP:
		OnLButtonUp(lParam);
		break;

	case WM_SETCURSOR:
		::SetCursor(m_hCursor);
		return 1L;	// Non-zero return prevents default processing

	case WM_NCHITTEST:
		return HTCLIENT;  // Claim that the mouse is in a client area
	}
	
	return 0L;
}


