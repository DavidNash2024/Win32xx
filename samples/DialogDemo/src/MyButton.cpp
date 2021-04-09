/////////////////////////////
// MyButton.cpp
//

#include "stdafx.h"
#include "MyButton.h"
#include "DialogDemoApp.h"
#include "UserMessages.h"

/////////////////////////////////
// CMyButton function definitions
//

// Called when the mouse is moved over the button control.
LRESULT CMyButton::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_MOUSEMOVE Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)_T("WM_MOUSEMOVE"));
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the mouse is detected over the button control.
LRESULT CMyButton::OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_NCHITTEST Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)_T("WM_NCHITTEST"));
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when a WM_SETCURSOR message is received.
LRESULT CMyButton::OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_SETCURSOR Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)_T("WM_SETCURSOR"));
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the left mouse button is pressed over the button.
LRESULT CMyButton::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_LBUTTONDOWN Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)_T("WM_LBUTTONDOWN"));
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the left mouse button is released over the button.
LRESULT CMyButton::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_LBUTTONUP Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)_T("WM_LBUTTONUP"));
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the right mouse button is pressed over the button.
LRESULT CMyButton::OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_RBUTTONDOWN Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)_T("WM_RBUTTONDOWN"));
    return FinalWindowProc(msg, wparam, lparam);
}

// Process the window messages for the button control.
LRESULT CMyButton::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_MOUSEMOVE:      return OnMouseMove(msg, wparam, lparam);
        case WM_NCHITTEST:      return OnNCHitTest(msg, wparam, lparam);
        case WM_SETCURSOR:      return OnSetCursor(msg, wparam, lparam);
        case WM_LBUTTONDOWN:    return OnLButtonDown(msg, wparam, lparam);
        case WM_LBUTTONUP:      return OnLButtonUp(msg, wparam, lparam);
        case WM_RBUTTONDOWN:    return OnRButtonDown(msg, wparam, lparam);
        default:
            break;
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}
