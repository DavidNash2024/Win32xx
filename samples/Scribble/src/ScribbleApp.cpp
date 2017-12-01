//////////////////////////////////
// ScribbleApp.cpp

#include "stdafx.h"
#include "ScribbleApp.h"


CScribbleApp::CScribbleApp()
{
}

BOOL CScribbleApp::InitInstance()
{
    //Create the Window
    m_Frame.Create();   // throws a CWinException on failure  

    return TRUE;
}
