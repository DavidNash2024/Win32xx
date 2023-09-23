/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "EdgeView.h"


// This function is the entry point for the application.
int WINAPI WinMain(__in HINSTANCE, __in_opt HINSTANCE, __in LPSTR, __in int)
{
    // Start Win32++.
    CWinApp app;

    // Create the CEdgeView window.
    CEdgeView edgeView;
    edgeView.Create();

    // Run the app's message loop.
    return app.Run();
}
