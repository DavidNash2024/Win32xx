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
CDialogApp::CDialogApp() : m_clientDialog(IDD_CLIENT)
{
}

// Destructor.
CDialogApp::~CDialogApp()
{
}

// Called when the application is about to start.
BOOL CDialogApp::InitInstance()
{
    // Display the Modal Dialog.
    m_clientDialog.DoModeless();    // throws a CWinException on failure

    return TRUE;
}



