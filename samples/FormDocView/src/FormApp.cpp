//////////////////////////////////////////////
// FormApp.cpp

#include "stdafx.h"
#include "FormApp.h"


////////////////////////////////
// CFormApp function definitions
//

// Called when the application starts.
BOOL CFormApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

