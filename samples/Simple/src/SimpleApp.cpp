/////////////////////////////
// SimpleApp.cpp
//

#include "SimpleApp.h"

///////////////////////////////////
// CSimpleApp function definitions.
//

// This function is called automatically when the application starts.
BOOL CSimpleApp::InitInstance()
{
    // Create the Window
    m_view.Create();

    return TRUE;
}
