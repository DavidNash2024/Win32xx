///////////////////////////////////////
// DlgSubclassApp.cpp

#include "resource.h"
#include "DlgSubclassApp.h"


// Definitions for the CDialogApp class
CDlgSubclassApp::CDlgSubclassApp() : m_myDialog(IDD_DIALOG1)
{
}

BOOL CDlgSubclassApp::InitInstance()
{
    //Display the Modal Dialog
    m_myDialog.DoModal();

    //End the program
    ::PostQuitMessage(0);

    return TRUE;
}

CDlgSubclassApp::~CDlgSubclassApp()
{
}

