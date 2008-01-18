/////////////////////////////////////////////////
//App.cpp -  Definitions for the CDialogApp class

#include "..\Win32++\dialog.h"
#include "resource.h"
#include "DialogApp.h"


CDialogApp::CDialogApp(HINSTANCE hInstance) : CWinApp(hInstance), MyDialog(IDD_DIALOG)
{
}

BOOL CDialogApp::InitInstance()
{
	//Display the Modal Dialog
	MyDialog.DoModal();

	//End the program
	::PostQuitMessage(0);

	return TRUE;
}

CDialogApp::~CDialogApp()
{
}

