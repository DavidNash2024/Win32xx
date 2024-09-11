/////////////////////////////
// Browser.cpp
//

#include "stdafx.h"
#include "Browser.h"
#include "resource.h"


////////////////////////////////
// CViewWeb function definitions
//

// Called when the window is created.
int CViewWeb::OnCreate(CREATESTRUCT& cs)
{
    CWebBrowser::OnCreate(cs);

    // Suppress Java script errors
    GetIWebBrowser2()->put_Silent(VARIANT_TRUE);

    // Load the web page.
    Navigate2(L"www.google.com");

    return 0;
}

// Handle the window's messages.
LRESULT CViewWeb::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
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
