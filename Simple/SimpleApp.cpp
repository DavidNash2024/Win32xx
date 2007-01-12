////////////////////////////////////////
// SimpleApp.h

#include "SimpleApp.h"


// Definitions for the CSimpleApp class
CSimpleApp::CSimpleApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
    //Create the Window
    m_View.Create();
}
