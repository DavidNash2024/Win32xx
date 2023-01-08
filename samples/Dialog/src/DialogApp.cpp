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
CDialogApp::CDialogApp() : m_myDialog(IDD_DIALOG1)
{
}

// Called when the application starts.
BOOL CDialogApp::InitInstance()
{
    // Display the modal dialog.
    m_myDialog.DoModal();   // throws a CWinException on failure.

    return TRUE;
}

// Destructor.
CDialogApp::~CDialogApp()
{
}

