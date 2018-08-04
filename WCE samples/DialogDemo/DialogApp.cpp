/////////////////////////////////////////////////
//App.cpp -  Definitions for the CDialogApp class

#include "wxx_dialog.h"
#include "resource.h"
#include "DialogApp.h"


CDialogApp::CDialogApp() : myDialog(IDD_COMCTLS)
{
}

BOOL CDialogApp::InitInstance()
{
    //Display the Modal Dialog
    myDialog.DoModal();

    //End the program
    ::PostQuitMessage(0);

    return TRUE;
}

CDialogApp::~CDialogApp()
{
}

