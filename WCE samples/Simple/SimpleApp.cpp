/////////////////////////////
// SimpleApp.cpp


#include "SimpleApp.h"


// Constructor.
CSimpleApp::CSimpleApp()
{
}

// Called when the application starts.
BOOL CSimpleApp::InitInstance()
{
    //Create the Window
    m_view.Create();

    return TRUE;
}