///////////////////////////////////////
// main.cpp

#include "wxx_wincore.h"

// Note:
//  * Add the Win32++\include  directory to project's additional include directories.


//////////////////////////////////////////////
// CMyWindow is the application's main window.
class CMyWindow : public CWnd
{
public:
    CMyWindow() = default;
    virtual void OnDestroy() override { PostQuitMessage(0); }    // End the program.
    virtual ~CMyWindow() override = default;
};


// WinMain is the program's entry point. The program starts here.
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    // Start Win32++.
    CWinApp theApp;

    // Create a CMyWindow object.
    CMyWindow myWindow;

    // Create (and display) the window.
    myWindow.Create();

    // Run the application's message loop.
    return theApp.Run();
}


