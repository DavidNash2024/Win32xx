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
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)L"WM_MOUSEMOVE");
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the mouse is detected over the button control.
LRESULT CMyButton::OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_NCHITTEST Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)L"WM_NCHITTEST");
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when a WM_SETCURSOR message is received.
LRESULT CMyButton::OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_SETCURSOR Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)L"WM_SETCURSOR");
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the left mouse button is pressed over the button.
LRESULT CMyButton::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_LBUTTONDOWN Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)L"WM_LBUTTONDOWN");
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the left mouse button is released over the button.
LRESULT CMyButton::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_LBUTTONUP Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)L"WM_LBUTTONUP");
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the right mouse button is pressed over the button.
LRESULT CMyButton::OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_RBUTTONDOWN Message\n");
    GetParent().SendMessage(UWM_SETSTATIC, (WPARAM)L"WM_RBUTTONDOWN");
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

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << '\n' << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}
