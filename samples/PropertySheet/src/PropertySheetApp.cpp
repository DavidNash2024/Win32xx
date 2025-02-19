/////////////////////////////
// PropertySheetApp.cpp
//
#include "stdafx.h"
#include "PropertySheetApp.h"

/////////////////////////////////////////
// CPropertySheetApp function definitions
//

// Called when the application starts.
BOOL CPropertySheetApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

