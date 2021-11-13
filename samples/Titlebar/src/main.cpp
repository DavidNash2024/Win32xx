/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "App.h"


int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    // Start Win32++
    CApp MyApp;

    // Run the application
    return MyApp.Run();
}


