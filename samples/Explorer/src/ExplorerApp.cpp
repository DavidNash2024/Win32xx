/////////////////////////////
// ExplorerApp.cpp
//

#include "stdafx.h"
#include "ExplorerApp.h"
#include "resource.h"

////////////////////////////////////
// CExplorerApp function definitions
//

// Destructor.
CExplorerApp::~CExplorerApp()
{
    ::CoUninitialize(); // Shut down COM
}

// Called when the application starts.
BOOL CExplorerApp::InitInstance()
{
    // Initialize COM
    if (SUCCEEDED(CoInitialize(nullptr)))
    {
        // Create the frame window.
        m_frame.Create();   // throws a CWinException on failure
    }
    else
    {
        ::MessageBox(nullptr, L"Failed to initialize COM", L"ERROR", MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

