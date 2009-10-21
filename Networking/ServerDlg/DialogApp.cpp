//////////////////////////////////////
// DialogApp.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"


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

