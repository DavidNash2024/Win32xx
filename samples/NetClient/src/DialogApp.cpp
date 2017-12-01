///////////////////////////////////////
// DialogApp.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"


// Definitions for the CDialogApp class
CDialogApp::CDialogApp() : m_ClientDialog(IDD_CLIENT)
{
}

BOOL CDialogApp::InitInstance()
{
    //Display the Modal Dialog
    m_ClientDialog.DoModeless();    // throws a CWinException on failure  

    return TRUE;
}

CDialogApp::~CDialogApp()
{
}

