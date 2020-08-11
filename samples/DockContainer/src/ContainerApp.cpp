/////////////////////////////
// ContainerApp.cpp
//

#include "stdafx.h"
#include "ContainerApp.h"


//////////////////////////////////////////////
// CDockContainerApp function definitions
//

// Constructor.
CDockContainerApp::CDockContainerApp()
{
}

// Destructor.
CDockContainerApp::~CDockContainerApp()
{
}

// Called when the application starts.
BOOL CDockContainerApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}


