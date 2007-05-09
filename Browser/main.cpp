/////////////////////////////////////////
// main.cpp


#include "FrameApp.h"



INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CFrameApp MyApp(hInstance);

	
	// Process the window messages
    return MyApp.MessageLoop();
}
