/////////////////////////////
// Button.cpp

#include "stdafx.h"
#include "MyButton.h"
#include "DialogDemoApp.h"


LRESULT CMyButton::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_MOUSEMOVE Message\n");
    CMyDialog& Dialog = GetDlgApp()->GetDialog();
    Dialog.SetStatic(_T("WM_MOUSEMOVE"));
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMyButton::OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_NCHITTEST Message\n");
    CMyDialog& Dialog = GetDlgApp()->GetDialog();
    Dialog.SetStatic(_T("WM_NCHITTEST"));
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMyButton::OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_SETCURSOR Message\n");
    CMyDialog& Dialog = GetDlgApp()->GetDialog();
    Dialog.SetStatic(_T("WM_SETCURSOR"));
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMyButton::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_LBUTTONDOWN Message\n");
    CMyDialog& Dialog = GetDlgApp()->GetDialog();
    Dialog.SetStatic(_T("WM_LBUTTONDOWN"));

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMyButton::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_LBUTTONUP Message\n");
    CMyDialog& Dialog = GetDlgApp()->GetDialog();
    Dialog.SetStatic(_T("WM_LBUTTONUP"));

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMyButton::OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TRACE("CButton::WndProc - WM_RBUTTONDOWN Message\n");
    CMyDialog& Dialog = GetDlgApp()->GetDialog();
    Dialog.SetStatic(_T("WM_RBUTTONDOWN"));

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMyButton::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
    //  TRACE("CButton::WndProc - Unspecified Message\n");
        break;
    }
    
    return WndProcDefault(msg, wparam, lparam);
}
