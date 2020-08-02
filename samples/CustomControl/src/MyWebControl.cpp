#include "stdafx.h"
#include "MyWebControl.h"

CWebControl::CWebControl()
{
    // The entry for the dialog's control in resource.rc must match this name.
    CString className = _T("WebControl");

    // Register the window class
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    HINSTANCE instance = GetApp()->GetInstanceHandle();
    if (!::GetClassInfo(instance, className, &wc))
    {
        wc.lpszClassName = className;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.hInstance = instance;
        ::RegisterClass(&wc);
    }

    assert (::GetClassInfo(instance, className, &wc));
}

void CWebControl::OnInitialUpdate()
{
    // Suppress Java script errors.
    GetIWebBrowser2()->put_Silent(VARIANT_TRUE);

    // Load web page.
    Navigate(_T("www.google.com"));
}



