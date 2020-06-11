///////////////////////////////////////
// main.cpp


#include "stdafx.h"
#include "MovieShowApp.h"


////////////////////////////////////////////////////////////
// Declaration of the CLimitSingleInstance class
// This code is from Microsoft KnowledgeBase Article Q243953.
class CLimitSingleInstance
{
protected:
    DWORD  m_lastError;
    HANDLE m_mutex;

public:
    CLimitSingleInstance(LPCWSTR mutexName)
    {
        // Make sure that you use a name that is unique for this application
        // otherwise two apps may think they are the same if they are using
        // same name for 3rd parm to CreateMutex.
        m_mutex = CreateMutex(NULL, FALSE, mutexName); // do early
        m_lastError = GetLastError(); // save for use later...
    }

    ~CLimitSingleInstance()
    {
        if (m_mutex)  // Do not forget to close handles.
        {
            CloseHandle(m_mutex); // Do as late as possible.
            m_mutex = NULL; // Good habit to be in.
        }
    }

    BOOL IsAnotherInstanceRunning()
    {
        return (ERROR_ALREADY_EXISTS == m_lastError);
    }
};

// Create an instance of CLimitSingleInstance.
CLimitSingleInstance g_singleInstanceObj(L"Global\\{2194ABA1-BFFA-4e6b-8C26-D1FABB16F9E6}");


// The application's entry point function.
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    try
    {
        if (g_singleInstanceObj.IsAnotherInstanceRunning())
        {
            ::MessageBox(NULL, L"Movie Show is already running", L"Warning", MB_ICONWARNING);
            return -1;
        }

        // Start Win32++
        CMovieShowApp theApp;

        // Run the application and the message loop
        return theApp.Run();
    }

    // catch all unhandled CException types
    catch (const CException &e)
    {
        // Display the exception and quit
        MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);
    }
    catch (...)
    {
        MessageBox(NULL, L"Got an unexpected exception", L"Error", MB_ICONERROR);
    }

    return -1;
}

