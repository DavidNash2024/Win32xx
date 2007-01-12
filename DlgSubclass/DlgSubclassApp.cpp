///////////////////////////////////////
// DlgSubclassApp.cpp

#include "resource.h"
#include "DlgSubclassApp.h"


// Definitions for the CDialogApp class
CDlgSubclassApp::CDlgSubclassApp(HINSTANCE hInstance) : CWinApp(hInstance), MyDialog(IDD_DIALOG1)
{
    //Start Trace
    TRACE("Start Tracing");

	//Display the Modal Dialog
	MyDialog.DoModal();

	//End the program
	::PostQuitMessage(0);
}

CDlgSubclassApp::~CDlgSubclassApp()
{
}

