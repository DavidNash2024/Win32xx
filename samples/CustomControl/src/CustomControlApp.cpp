///////////////////////////////////////
// CustomControlApp.cpp

#include "stdafx.h"
#include "CustomControlApp.h"
#include "resource.h"


// Definitions for the CDialogApp class
CCustomControlApp::CCustomControlApp() : m_myDialog(IDD_DIALOG1)
{
}

CCustomControlApp::~CCustomControlApp()
{
}

BOOL CCustomControlApp::InitInstance()
{
    //Display the Modal Dialog
    m_myDialog.DoModal();   // throws a CWinException on failure

    return TRUE;
}

