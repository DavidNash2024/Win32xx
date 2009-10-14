///////////////////////////////////////
// DlgSubclassApp.cpp

#include "resource.h"
#include "DialogDemoApp.h"


// Definitions for the CDialogApp class
CDlgSubclassApp::CDlgSubclassApp() : m_MyDialog(IDD_DIALOG1)
{
}

CDlgSubclassApp::~CDlgSubclassApp()
{
}

BOOL CDlgSubclassApp::InitInstance()
{
	//Display the Modal Dialog
	m_MyDialog.DoModal();

	//End the program
	::PostQuitMessage(0);

	return TRUE;
}

