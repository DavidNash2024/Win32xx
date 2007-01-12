//////////////////////////////////////
// MDIFrameApp.cpp


#include "resource.h"
#include "MDIFrameApp.h"


CMDIFrameApp::CMDIFrameApp(HINSTANCE hInstance) : CMDIApp(hInstance)
{
	//Start Tracing
	TRACE("Start Tracing");

    //Create the Window
    m_MainMDIFrame.Create();
}


