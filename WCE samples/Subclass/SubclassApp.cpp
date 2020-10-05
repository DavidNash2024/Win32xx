///////////////////////////////////////////////
//SubclassApp.cpp -  Definitions for the CSubclassApp class


#include "SubclassApp.h"


// Constructor.
CSubclassApp::CSubclassApp()
{
}

// Called when the application starts.
BOOL CSubclassApp::InitInstance()
{
    TRACE("Move the mouse over the button to see messages");

    //Create the Outer Window
    m_win.Create();

    return TRUE;
}

// Destructor.
CSubclassApp::~CSubclassApp()
{
}
