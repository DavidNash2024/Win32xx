////////////////////////////////
// Main.cpp


#include "ScribbleApp.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CScribbleApp MyApp(hInstance);

	// Run the application
    return MyApp.Run();
}
