/////////////////////////////////////////
// main.cpp

#include "SimpleApp.h"


INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CSimpleApp MyApp;

	// Run the application
    return MyApp.Run();
}


// Note:  For the purpose of simplicity, this sample is not Unicode compliant.
//        (If you're not sure what Unicode means, then it doesn't matter.)

