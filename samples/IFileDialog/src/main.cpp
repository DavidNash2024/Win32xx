/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "IFileDialogApp.h"


#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning( suppress : 28251 )  // Ignore the annotation requirement for wWinMain.
#endif

//#include "C:\Program Files (x86)\Visual Leak Detector\include\vld.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    try
    {
        // Start Win32++.
        CIFileDialogApp theApp;

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
        str2 << "Error: " << e.what();
        TaskDialogBox(nullptr, str1, str2, TD_ERROR_ICON);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        TaskDialogBox(nullptr, str1, L"Error: std::exception", TD_ERROR_ICON);
    }

    return -1;
}

