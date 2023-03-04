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
    Navigate2(_T("www.google.com"));

    return 0;
}