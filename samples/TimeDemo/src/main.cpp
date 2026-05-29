/////////////////////////////////////////
// main.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "App.h"

// Application entry point.
int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    // Semaphore name, instances, and  handle: make sure the name is  unique to
    // this application.
    const CString szSemaphoreName = L"Unique name: TimeDemo";

    // Number of allowed instances: here, 1 avoids archive file clashes.
    const int nInstances = 1;

    // Set default return value.
    int rtn = -1;

    // Create and the semaphore that limits the number of simultaneously
    // executing instances of this application to m_nInstances.
    CSemaphore semaphore(nInstances, nInstances, szSemaphoreName);
    {
        if (WaitForSingleObject(semaphore, 0) == WAIT_TIMEOUT)
        {
            ::MessageBox(nullptr,
                L"The allowed number of instances of this\n"
                L"application are already running.",
                L"Stop", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
                return 0;
        }
        try
        {
            CApp thisApp;

            // Run the application.
            rtn = thisApp.Run();
        }

        catch(const CException& e)
        {
            CString msg;
            CString what(e.what());
            msg.Format(L"%s\n%s\n%s", e.GetText(), e.GetText(),
                e.GetErrorString(), L"\nWinMain Goodbye...");
            ::MessageBox(nullptr, msg.c_str(), what.c_str(),
                MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        }
        catch(...)
        {
            ::MessageBox(nullptr, L"WinMain Goodbye...", L"Error",
                MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
        }
    }

    return rtn;
}
