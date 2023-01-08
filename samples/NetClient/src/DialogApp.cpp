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

// Called when the application starts.
BOOL CDialogApp::InitInstance()
{
    // Display the modal dialog.
    m_clientDialog.DoModeless();    // throws a CWinException on failure

    return TRUE;
}

