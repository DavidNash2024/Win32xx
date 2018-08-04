//////////////////////////////////////////////
//PictureApp.cpp

#include "stdafx.h"
#include "PictureApp.h"


// Definitions for the CFrameApp class
CPictureApp::CPictureApp()
{
}

CPictureApp::~CPictureApp()
{
}

BOOL CPictureApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure  

    return TRUE;
}
