//////////////////////////////////////
// DialogApp.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"


// Definitions for the CDialogApp class
CDialogApp::CDialogApp() : m_SvrDialog(IDD_SERVER)
{
}

BOOL CDialogApp::InitInstance()
{
	//Display the Modal Dialog
	m_SvrDialog.DoModeless();

	return TRUE;
}

CDialogApp::~CDialogApp()
{
}

