/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "MovieShowApp.h"


////////////////////////////////////////////////////////////
// CLimitSingleInstance uses a mutex to ensure that only
// a single instance of the application runs.
// This code is from Microsoft KnowledgeBase Article Q243953.
class CLimitSingleInstance
{
public:
    CLimitSingleInstance(LPCWSTR mutexName)
    {
        // Make sure that you use a name that is unique for this application
        // otherwise two apps may think they are the same if they are using
        // same name for 3rd parameter to CreateMutex.
        m_mutex = ::CreateMutex(nullptr, FALSE, mutexName); // do early
        m_lastError = GetLastError(); // save for use later...
    }

    ~CLimitSingleInstance()
    {
        if (m_mutex)  // Do not forget to close handles.
        {
            CloseHandle(m_mutex);    // Do as late as possible.
            m_mutex = nullptr;       // Good habit to be in.
        }
    }

    BOOL IsAnotherInstanceRunning()
    {
        return (ERROR_ALREADY_EXISTS == m_lastError);
    }

private:
    DWORD  m_lastError;
    HANDLE m_mutex;
};

// Create an instance of CLimitSingleInstance.
CLimitSingleInstance g_singleInstanceObj(L"Global\\{2194ABA1-BFFA-4e6b-8C26-D1FABB16F9E6}");


// The application's entry point function.
int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    try
    {
        if (g_singleInstanceObj.IsAnotherInstanceRunning())
        {
            ::MessageBox(nullptr, L"Movie Show is already running", L"Warning", MB_ICONWARNING);
            return -1;
        }

        // Start Win32++.
        CMovieShowApp theApp;

        // Run the application and the message loop.
        return theApp.Run();
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return -1;
}

