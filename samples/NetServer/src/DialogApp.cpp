//////////////////////////////////////
// DialogApp.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"


// Definitions for the CDialogApp class
CDialogApp::CDialogApp() : m_svrDialog(IDD_SERVER)
{
}

BOOL CDialogApp::InitInstance()
{
    //Display the Modal Dialog
    m_svrDialog.DoModeless();   // throws a CWinException on failure

    return TRUE;
}

CDialogApp::~CDialogApp()
{
}

