///////////////////////////////////////
// CustomControlApp.cpp

#include "stdafx.h"
#include "resource.h"
#include "CustomControlApp.h"


// Definitions for the CDialogApp class
CCustomControlApp::CCustomControlApp() : m_MyDialog(IDD_DIALOG1)
{
}

CCustomControlApp::~CCustomControlApp()
{
}

BOOL CCustomControlApp::InitInstance()
{
	//Display the Modal Dialog
	m_MyDialog.DoModal();

	return TRUE;
}

