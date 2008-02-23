//////////////////////////////////////
// DialogApp.cpp

#include "resource.h"
#include "DialogApp.h"


// Definitions for the CDialogApp class
CDialogApp::CDialogApp() : m_SvrDialog(IDD_DIALOG1)
{
}

BOOL CDialogApp::InitInstance()
{
	//Display the Modal Dialog
	m_SvrDialog.DoModal();

	//End the program
	::PostQuitMessage(0);

	return TRUE;
}

CDialogApp::~CDialogApp()
{
}

