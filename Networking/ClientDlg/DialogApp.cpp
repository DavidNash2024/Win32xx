///////////////////////////////////////
// DialogApp.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"


// Definitions for the CDialogApp class
CDialogApp::CDialogApp() : m_ClientDialog(IDD_DIALOG1)
{
}

BOOL CDialogApp::InitInstance()
{
	//Display the Modal Dialog
	m_ClientDialog.DoModeless();

	return TRUE;
}

CDialogApp::~CDialogApp()
{
}

