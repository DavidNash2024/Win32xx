//////////////////////////////////
// main.cpp

// To configure your project:
// 1) Add Win32++'s include directory to the project's additional include directories for C/C++
// 2) Add Win32++'s include directory to the project's additional include directories for Resources

#include "wxx_wincore.h"


// A class that inherits from CWnd. It is used to create the window.
class CView : public CWnd
{
public:
    CView() {}
    virtual ~CView() {}
    virtual void OnDestroy() { PostQuitMessage(0); }	// Ends the program
};



int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CWinApp theApp;

    // Create our view window
    CView MyWin;
    MyWin.Create();

    // Run the application's message loop
    return theApp.Run();
}
