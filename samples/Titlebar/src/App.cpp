/////////////////////////////
// App.cpp
//

#include "wxx_wincore.h"
#include "App.h"

///////////////////////////////////
// CApp function definitions.
//
CApp::CApp()
{
    // Constructor for CApp
}

BOOL CApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Window
    m_view.Create();

    return TRUE;
}
