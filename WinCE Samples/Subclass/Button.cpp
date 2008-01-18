/////////////////////////////
// Button.cpp


#include "Button.h"


CButton::~CButton()
{
}

void CButton::PreCreate(CREATESTRUCT &cs)
{
	cs.lpszClass = TEXT("BUTTON");
	cs.style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
	cs.x = 10;
	cs.y = 10;
	cs.cx = 100;
	cs.cy = 100;
	cs.lpszName = TEXT("OK");
}

LRESULT CButton::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		TRACE(TEXT("WndProc - WM_MOUSEMOVE Message"));
		break;
	case WM_SETCURSOR:
		TRACE(TEXT("WndProc - WM_SETCURSOR Message"));
		break;
	case WM_LBUTTONDOWN:
		TRACE(TEXT("WndProc - WM_LBUTTONDOWN Message"));
		break;
	case WM_LBUTTONUP:
		TRACE(TEXT("WndProc - WM_LBUTTONUP Message"));
		break;
	case WM_PAINT:
		TRACE(TEXT("WndProc - WM_PAINT Message"));
		break;

	default:
		TRACE(TEXT("WndProc - Unspecified Message"));
		break;
	}

	// Pass unhandled messages on for default processing
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}
