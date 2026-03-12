/////////////////////////////
// main.cpp
//

#include "SimpleApp.h"

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    // Start Win32++.
    CSimpleApp MyApp;

    // Run the application.
    return MyApp.Run();
}



