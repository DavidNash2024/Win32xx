/////////////////////////////
// main.cpp
//

#include "SimpleApp.h"


// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CSimpleApp MyApp;

    // Run the application
    return MyApp.Run();
}



