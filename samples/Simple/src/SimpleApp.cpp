/////////////////////////////
// SimpleApp.cpp
//

#include "SimpleApp.h"

///////////////////////////////////
// CSimpleApp function definitions.
//
CSimpleApp::CSimpleApp()
{
    // Constructor for CSimpleApp
}

BOOL CSimpleApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Window
    m_view.Create();

    return TRUE;
}
