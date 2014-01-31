///////////////////////////////////////
// DlgSubclassApp.cpp

#include "stdafx.h"
#include "resource.h"
#include "DialogDemoApp.h"


// Definitions for the CDialogApp class
CDialogDemoApp::CDialogDemoApp() : m_MyDialog(IDD_DIALOG1)
{
}

CDialogDemoApp::~CDialogDemoApp()
{
}

BOOL CDialogDemoApp::InitInstance()
{
	//Display the Modal Dialog
	m_MyDialog.DoModal();

	return TRUE;
}

