//////////////////////////////////////
// MDIDemoApp.h


#include "resource.h"
#include "MDIDemoApp.h"


CMDIDemoApp::CMDIDemoApp(HINSTANCE hInstance) : CMDIApp(hInstance)
{
	//Start Tracing
	TRACE("Start Tracing");

    //Create the Window
    m_MainMDIFrame.Create();
}


