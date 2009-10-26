///////////////////////////////////////
// DialogApp.cpp

#include "stdafx.h"
#include "resource.h"
#include "DialogApp.h"


// Definitions for the CDialogApp class
CDialogApp::CDialogApp() : m_MyDialog(IDD_DIALOG1)
{
}

CDialogApp::~CDialogApp()
{
}

BOOL CDialogApp::InitInstance()
{
	//Display the Modeless Dialog
	m_MyDialog.Create();

	return TRUE;
}

