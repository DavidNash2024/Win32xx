///////////////////////////////////////
// DlgSubclassApp.cpp

#include "stdafx.h"
#include "DialogDemoApp.h"
#include "resource.h"


// Definitions for the CDialogApp class
CDialogDemoApp::CDialogDemoApp() : m_myDialog(IDD_DIALOG1)
{
}

CDialogDemoApp::~CDialogDemoApp()
{
}

BOOL CDialogDemoApp::InitInstance()
{
    //Display the Modal Dialog
    m_myDialog.DoModal();   // throws a CWinException on failure

    return TRUE;
}

