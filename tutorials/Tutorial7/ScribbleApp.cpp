//////////////////////////////////////
// ScribbleApp.h

#include "targetver.h"
#include "ScribbleApp.h"

CScribbleApp::CScribbleApp()
{
}

BOOL CScribbleApp::InitInstance()
{
    //Create the Window
    m_Frame.Create();

    return TRUE;
}
