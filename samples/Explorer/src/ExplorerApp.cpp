/////////////////////////////
// ExplorerApp.cpp
//

#include "stdafx.h"
#include "ExplorerApp.h"
#include "resource.h"

////////////////////////////////////
// CExplorerApp function definitions
//

// Constructor.
CExplorerApp::CExplorerApp()
{
}

// Destructor.
CExplorerApp::~CExplorerApp()
{
    ::CoUninitialize(); // Shut down COM
}

// Called when the application starts.
BOOL CExplorerApp::InitInstance()
{
    // Initialize COM
    if (SUCCEEDED(CoInitialize(NULL)))
    {
        // Create the frame window.
        m_frame.Create();   // throws a CWinException on failure
    }
    else
    {
        ::MessageBox(0, _T("Failed to initialize COM"), _T("ERROR"), MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

