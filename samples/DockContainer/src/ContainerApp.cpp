/////////////////////////////
// ContainerApp.cpp
//

#include "stdafx.h"
#include "ContainerApp.h"


//////////////////////////////////////////////
// CDockContainerApp function definitions
//

// Called when the application starts.
BOOL CDockContainerApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}


