//////////////////////////////////////////////
//PropertySheetApp.cpp

#include "stdafx.h"
#include "PropertySheetApp.h"


// Definitions for the CFrameApp class
CPropertySheetApp::CPropertySheetApp()
{
}

CPropertySheetApp::~CPropertySheetApp()
{
}

BOOL CPropertySheetApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure  

    return TRUE;
}

