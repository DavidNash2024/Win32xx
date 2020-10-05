/////////////////////////////////////////////////
//App.cpp -  Definitions for the CDialogApp class

#include "wxx_dialog.h"
#include "resource.h"
#include "DialogApp.h"


// Constructor.
CDialogApp::CDialogApp() : myDialog(IDD_DIALOG)
{
}

// Called when the application starts.
BOOL CDialogApp::InitInstance()
{
    //Display the Modal Dialog
    myDialog.DoModal();

    //End the program
    ::PostQuitMessage(0);

    return TRUE;
}

// Destructor.
CDialogApp::~CDialogApp()
{
}

