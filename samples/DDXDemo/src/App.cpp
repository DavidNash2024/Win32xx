/////////////////////////////////////////
// App.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "App.h"

// The WINAPI entry point for a graphical Windows-based application.
int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    try
    {
        // Declare the application object.
        CApp theApp;

        // Run the application.
        return theApp.Run();
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << L"Error: " << e.what();
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

/////////////////////////////
// CApp function definitions.
//

// Called by theApp.Run() execution in WinMain to create the application
// frame, which controls the program.
BOOL CApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // Throws a CWinException on failure.
    return TRUE;
}
