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
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return -1;
}

