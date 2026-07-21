/////////////////////////////////////////
// main.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "pch.h"
#include "App.h"

// Application entry point.
int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    int rtn = -1;

    // Run the application
    CApp thisApp;
    try
    {
        rtn = thisApp.Run();
    }
    catch (const CException &e)   // catch all CException events
    {
        // Process the exception and quit
        CString msg;
        msg << e.what() << '\n' << e.GetText() << "\nWinMain Goodbye...";
        ::MessageBox(nullptr, msg, L"Standard Exception", MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(...)      // catch all other exception events
    {
        CString msg = L"Unregistered exception event.\nWinMain Goodbye...";
        ::MessageBox(nullptr, msg, L"Unknown Exception", MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }

    return rtn;
}
