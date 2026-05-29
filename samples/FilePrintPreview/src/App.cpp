/////////////////////////////////////////
// App.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "App.h"

/////////////////////////////
// CApp function definitions.
//

// Called by the framework to create the frame.
BOOL CApp::InitInstance()
{
    m_frame.Create();   // Throws a CWinException on failure.
    return TRUE;
}
