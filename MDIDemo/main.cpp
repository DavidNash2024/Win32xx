///////////////////////////////////////
// main.cpp


#include "MDIDemoApp.h"

INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
    CMDIDemoApp MyApp(hInstance);

    // Process the window messages
	return MyApp.MessageLoop();
}

