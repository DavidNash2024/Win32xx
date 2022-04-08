/////////////////////////////
// main.cpp
//

// VS6 requires these macros to be defined.
#define WINVER          0x0410
#define _WIN32_WINDOWS  0x0410
#define _WIN32_IE       0x0401

#include "RoundApp.h"


// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        // Start Win32++
        CRoundApp MyApp;

        // Run the application
        return MyApp.Run();
    }

    // Catch all unhandled CException types.
    catch (const CException &e)
    {
        // Display the exception and quit.
        MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }
}



