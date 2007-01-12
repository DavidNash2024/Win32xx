///////////////////////////////////////
// DialogApp.cpp

#include "resource.h"
#include "DialogApp.h"


// Definitions for the CDialogApp class
CDialogApp::CDialogApp(HINSTANCE hInstance) : CWinApp(hInstance), MyDialog(IDD_DIALOG1)
{
    //Start Trace
    TRACE("Start Tracing");

	//Display the Modal Dialog
	MyDialog.DoModal();

	//End the program
	::PostQuitMessage(0);
}

CDialogApp::~CDialogApp()
{
}

