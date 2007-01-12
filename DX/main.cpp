///////////////////////////////////////
// main.cpp

#include "..\Win32++\Wincore.h"
#include "DXApp.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CDXApp MyApp(hInstance);

	// Process the window messages
	return MyApp.MessageLoop();
}
