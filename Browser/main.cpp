/////////////////////////////////////////
// main.cpp

#include "BrowserApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CBrowserApp MyApp(hInstance);


	// Run the application
    return MyApp.Run();
}
