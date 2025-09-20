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
