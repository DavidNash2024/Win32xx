/////////////////////////////
// SimpleApp.cpp
//

#include "SimpleApp.h"

///////////////////////////////////
// CSimpleApp function definitions.
//

// Constructor for CSimpleApp.
CSimpleApp::CSimpleApp()
{
}

// This function is called automatically when the application starts.
BOOL CSimpleApp::InitInstance()
{
    // Create the Window
    m_view.Create();

    return TRUE;
}
