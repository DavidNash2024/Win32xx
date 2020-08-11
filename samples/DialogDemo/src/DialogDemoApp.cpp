/////////////////////////////
// DialogDemoApp.cpp
//

#include "stdafx.h"
#include "DialogDemoApp.h"
#include "resource.h"


//////////////////////////////////////
// CDialogDemoApp function definitions
//

// Constructor.
CDialogDemoApp::CDialogDemoApp() : m_myDialog(IDD_DIALOG1)
{
}

// Destructor.
CDialogDemoApp::~CDialogDemoApp()
{
}

// Called when the application starts.
BOOL CDialogDemoApp::InitInstance()
{
    //Display the modal dialog
    m_myDialog.DoModal();   // throws a CWinException on failure

    return TRUE;
}

