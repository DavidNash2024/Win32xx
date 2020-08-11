/////////////////////////////
// DXApp.cpp
//

#include "stdafx.h"
#include "DXApp.h"

//////////////////////////////
// CDXApp function definitions
//

// Constructor.
CDXApp::CDXApp()
{
}

// Called when the application starts.
BOOL CDXApp::InitInstance()
{
    //Create the Window
    m_frame.Create();

    return TRUE;
}

