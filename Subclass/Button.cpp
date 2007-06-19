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

void CButton::OnInitialUpdate()
{
	//Subclass the window to handle additional messages
	Subclass();
}

LRESULT CButton::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_NCPAINT:
		TRACE("CButton::WndProc - WM_NCPAINT Message");
		break;
	case WM_PAINT:
		TRACE("CButton::WndProc - WM_PAINT Message");
		break;

	default:
		TRACE("CButton::WndProc - Unspecified Message");
		break;
	}
	// Now hand all messages to the original Window procedure
	return CWnd::WndProc(hWnd, uMsg, wParam, lParam);
}
