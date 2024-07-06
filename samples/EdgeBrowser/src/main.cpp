/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "EdgeView.h"


// This function is the entry point for the application.
int WINAPI WinMain(__in HINSTANCE, __in_opt HINSTANCE, __in LPSTR, __in int)
{
    try
    {
        // Start Win32++.
        CWinApp app;

        // Create the CEdgeView window.
        CEdgeView edgeView;
        edgeView.Create();

        // Run the app's message loop.
        return app.Run();
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
