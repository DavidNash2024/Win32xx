///////////////////////////////////////
// main.cpp

#include "DXApp.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CDXApp MyApp(hInstance);

	// Run the application
	return MyApp.Run();
}
