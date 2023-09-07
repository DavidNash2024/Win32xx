/////////////////////////////
// EdgeApp.cpp
//

#include "stdafx.h"
#include "EdgeApp.h"


///////////////////////////////////
// CEdgeApp function definitions.
//

// Constructor for CEdgeApp.
CEdgeApp::CEdgeApp()
{
}

// This function is called automatically when the application starts.
BOOL CEdgeApp::InitInstance()
{
    // Create the Window
    m_view.Create();

    return TRUE;
}
