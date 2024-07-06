/////////////////////////////
// MyWebControl.cpp
//

#include "stdafx.h"
#include "MyWebControl.h"

////////////////////////////////////////
// Definitions for the CWebControl class
//

// Constructor
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

// Called after the web control is created.
void CWebControl::OnInitialUpdate()
{
    // Suppress Java script errors.
    GetIWebBrowser2()->put_Silent(VARIANT_TRUE);

    // Load web page.
    Navigate2(_T("www.google.com"));
}

// Handle the window's messages.
LRESULT CWebControl::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}

