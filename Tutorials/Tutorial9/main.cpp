////////////////////////////////
// Main.cpp

#include "ScribbleApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CScribbleApp MyApp;

	// Run the application
    return MyApp.Run();
}
