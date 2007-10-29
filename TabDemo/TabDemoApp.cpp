////////////////////////////////////////
// TabDemoApp.h

#include "TabDemoApp.h"


// Definitions for the CTabDemoApp class
CTabDemoApp::CTabDemoApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
    //Create the Window
    m_View.Create();
}
