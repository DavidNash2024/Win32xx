///////////////////////////////////////
// DialogApp.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"


// Definitions for the CDialogApp class
CDialogApp::CDialogApp() : m_myDialog(IDD_DIALOG1)
{
}

BOOL CDialogApp::InitInstance()
{
    //Display the Modal Dialog
    m_myDialog.DoModal();   // throws a CWinException on failure  

    return TRUE;
}

CDialogApp::~CDialogApp()
{
}

