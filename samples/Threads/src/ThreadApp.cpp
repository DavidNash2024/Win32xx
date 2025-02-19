/////////////////////////////
//ThreadApp.cpp
//

#include "stdafx.h"
#include "ThreadApp.h"

///////////////////////////////////
// CThreadApp function definitions.
//

// Called when the application starts.
BOOL CThreadApp::InitInstance()
{
    m_mainWnd.Create(); // throws a CWinException on failure

    return TRUE;
}
