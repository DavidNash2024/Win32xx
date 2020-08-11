/////////////////////////////
// DialogApp.cpp
//

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"

//////////////////////////////////
// CDialogApp function definitions
//

CDialogApp::CDialogApp() : m_myDialog(IDD_DIALOG1)
{
}

CDialogApp::~CDialogApp()
{
}

BOOL CDialogApp::InitInstance()
{
    //Display the modeless dialog.
    m_myDialog.Create();    // throws a CWinException on failure.

    return TRUE;
}

