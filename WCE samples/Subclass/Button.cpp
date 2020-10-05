/////////////////////////////
// Button.cpp


#include "Button.h"


// Destructor.
CButton::~CButton()
{
}

// Sets the CREATESTRUCT parameters before the window is created.
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

// Handle the button window's messages.
LRESULT CButton::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
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

    // Pass unhandled messages on for default processing.
    return CWnd::WndProc(msg, wparam, lparam);
}
