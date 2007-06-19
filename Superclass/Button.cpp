/////////////////////////////
// Button.cpp


#include "Button.h"

CButton::CButton()
{
	// This is where we superclass the button control
	Superclass(TEXT("BUTTON"), TEXT("SuperButton"));
}

CButton::~CButton()
{
}

void CButton::PreCreate(CREATESTRUCT &cs)
{
	cs.lpszClass = TEXT("SuperButton");
	cs.style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
	cs.x = 10;
	cs.y = 10;
	cs.cx = 100;
	cs.cy = 100;
	cs.lpszName = TEXT("Push me");
}

LRESULT CButton::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Display window creation messages, and left mouse button clicks
	switch (uMsg)
	{
	case WM_CREATE:
		TRACE("CButton::WndProc - WM_CREATE Message");
		break;
	case WM_LBUTTONDBLCLK:
		TRACE("CButton::WndProc - WM_LBUTTONDBLCLK Message");
		break;
	case WM_LBUTTONDOWN:
		TRACE("CButton::WndProc - WM_LBUTTONDOWN Message");
		break;
	case WM_MOVE:
		TRACE("CButton::WndProc - WM_MOVE Message");
		break;
	case WM_NCCALCSIZE:
		TRACE("CButton::WndProc - WM_NCCALCSIZE Message");
		break;
	case WM_NCCREATE:
		TRACE("CButton::WndProc - WM_NCCREATE Message");
		break;
	case WM_NCPAINT:
		TRACE("CButton::WndProc - WM_NCPAINT Message");
		break;
	case WM_PAINT:
		TRACE("CButton::WndProc - WM_PAINT Message");
		break;
	case WM_SIZE:
		TRACE("CButton::WndProc - WM_SIZE Message");
		break;
	}
	// Now hand all messages to the original Window procedure
	return CWnd::WndProc(hWnd, uMsg, wParam, lParam);
}
