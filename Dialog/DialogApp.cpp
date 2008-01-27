///////////////////////////////////////
// DialogApp.cpp

#include "resource.h"
#include "DialogApp.h"


// Definitions for the CDialogApp class
CDialogApp::CDialogApp(HINSTANCE hInstance) : CWinApp(hInstance), m_MyDialog(IDD_DIALOG1)
{
}

BOOL CDialogApp::InitInstance()
{
	//Display the Modal Dialog
	m_MyDialog.DoModal();

	//End the program
	::PostQuitMessage(0);

	return TRUE;
}

CDialogApp::~CDialogApp()
{
}

