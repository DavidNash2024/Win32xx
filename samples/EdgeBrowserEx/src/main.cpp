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
    // Start Win32++.
    CWinApp app;

    // Create the browser window.
    CBrowserWindow browserWindow;
    browserWindow.Create();

    // Run the app's message loop.
    return app.Run();
}

