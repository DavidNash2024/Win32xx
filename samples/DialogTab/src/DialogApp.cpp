///////////////////////////////////////
// DialogApp.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"


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
	// A modeless dialog gives us an opportunity to pretranslatate messages 
	m_MyDialog.Create();

	return TRUE;
}

