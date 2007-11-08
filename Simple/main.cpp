/////////////////////////////////////////
// main.cpp

#include "SimpleApp.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CSimpleApp MyApp(hInstance);

	// Run the application
    return MyApp.Run();
}
