/////////////////////////////
// main.cpp
//

// To configure your project:
// 1) Add Win32++'s include directory to the project's additional include directories for C/C++
// 2) Add Win32++'s include directory to the project's additional include directories for Resources

// Include the minimal core set of features for creating windows.
#include "wxx_wincore.h"

// Alternatively, include all library features.
// #include "stdafx.h"


/////////////////////////////////////////////////
// CView inherits from CWnd. It is used to create
// the window.
class CView : public CWnd
{
public:
    CView() {}
    virtual ~CView() override {}
    virtual void OnDestroy() override { PostQuitMessage(0); }    // Ends the program
};


int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    // Start Win32++.
    CWinApp theApp;

    // Create our view window.
    CView myWin;
    myWin.Create();

    // Run the application's message loop.
    return theApp.Run();
}
