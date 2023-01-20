/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "FrameApp.h"


int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    try
    {
        if (GetWinVersion() < 3000)
            throw CUserException(L"Windows 10 or higher required");

        // Start Win32++.
        CFrameApp theApp;

        // Run the application and the message loop.
        return theApp.Run();
    }

    // Catch all unhandled CException types.
    catch (const CException &e)
    {
        // Display the exception and quit.
        ::MessageBox(0, e.GetText(), L"Error", MB_ICONERROR);

        return -1;
    }
}

