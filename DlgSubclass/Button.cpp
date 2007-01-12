/////////////////////////////
// Button.cpp


#include "Button.h"

LRESULT CButton::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		TRACE("CButton::WndProc - WM_MOUSEMOVE Message");
		break;
	case WM_NCHITTEST:
		TRACE("CButton::WndProc - WM_NCHITTEST Message");
		break;
	case WM_SETCURSOR:
		TRACE("CButton::WndProc - WM_SETCURSOR Message");
		break;
	case WM_LBUTTONDOWN:
		TRACE("CButton::WndProc - WM_LBUTTONDOWN Message");
		break;
	case WM_LBUTTONUP:
		TRACE("CButton::WndProc - WM_LBUTTONUP Message");
		break;	

	default:
//		TRACE("CButton::WndProc - Unspecified Message");
		break;
	}
	// Now hand all messages to the default CWnd window procedure
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}
