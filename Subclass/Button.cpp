/////////////////////////////
// Button.cpp


#include "Button.h"


CButton::~CButton()
{
}

void CButton::PreCreate(CREATESTRUCT &cs)
{
	// Choose "BUTTON as the window class. This is a predefined window class
	//  which creates a button window. Since its a predefined window class, Win32++ 
	//  automatically subclasses it, so the window messages are passed via WndProc
	cs.lpszClass = _T("BUTTON");

	// Choose other reasonable creation parameters
	cs.style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
	cs.x = 10;
	cs.y = 10;
	cs.cx = 100;
	cs.cy = 100;
	cs.lpszName = _T("OK");
}

void CButton::OnInitialUpdate()
{
}

LRESULT CButton::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Run this application in Debug mode so you can see the text displayed 
	//  in the Trace window when the mouse is over the button.
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
	return 0L;
}
