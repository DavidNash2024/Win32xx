/////////////////////////////
// DialogApp.cpp
//

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"

//////////////////////////////////
// CDialogApp function definitions
//

// Constructor.
CDialogApp::CDialogApp() : m_svrDialog(IDD_SERVER)
{
}

// Destructor.
CDialogApp::~CDialogApp()
{
}

// Called when the application starts.
BOOL CDialogApp::InitInstance()
{
    //Display the Modal Dialog
    m_svrDialog.DoModeless();   // throws a CWinException on failure

    return TRUE;
}



