//////////////////////////////////////////////////////
// Browser.cpp -  Definitions for the CViewWeb class

#include "stdafx.h"
#include "Browser.h"
#include "resource.h"



///////////////////////////////////////////////
// CViewWeb functions
int CViewWeb::OnCreate(CREATESTRUCT& cs)
{
    CWebBrowser::OnCreate(cs);

    // Suppress Java script errors
    GetIWebBrowser2()->put_Silent(VARIANT_TRUE);

    // Load the Win32++ home page
    Navigate2(_T("http://sourceforge.net/projects/win32-framework/"));

    return 0;
}