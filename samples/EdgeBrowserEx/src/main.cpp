// Copyright (C) Microsoft Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BrowserWindow.h"


int APIENTRY wWinMain(_In_ HINSTANCE,
    _In_opt_ HINSTANCE,
    _In_ LPWSTR,
    _In_ int)
{
    try
    {
        // Start Win32++.
        CWinApp app;

        // Create the browser window.
        CBrowserWindow browserWindow;
        browserWindow.Create();

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

