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

LRESULT CButton::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		TRACE("WndProc - WM_MOUSEMOVE Message\n");
		break;
	case WM_SETCURSOR:
		TRACE("WndProc - WM_SETCURSOR Message\n");
		break;
	case WM_LBUTTONDOWN:
		TRACE("WndProc - WM_LBUTTONDOWN Message\n");
		break;
	case WM_LBUTTONUP:
		TRACE("WndProc - WM_LBUTTONUP Message\n");
		break;
	case WM_PAINT:
		TRACE("WndProc - WM_PAINT Message\n");
		break;

	default:
		TRACE("WndProc - Unspecified Message\n");
		break;
	}

	// Pass unhandled messages on for default processing
	return CWnd::WndProc(uMsg, wParam, lParam);
}
