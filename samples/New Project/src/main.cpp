/////////////////////////////
// main.cpp
//

// To configure your project:
// 1) Add Win32++'s include directory to the project's additional include directories for C/C++
// 2) Add Win32++'s include directory to the project's additional include directories for Resources

// include the minimal core set of features for creating windows
#include "wxx_wincore.h"

// alternatively include all library features
// #include "stdafx.h"


/////////////////////////////////////////////////
// CView inherits from CWnd. It is used to create
// the window.
class CView : public CWnd
{
public:
    CView() {}
    virtual ~CView() {}
    virtual void OnDestroy() { PostQuitMessage(0); }    // Ends the program
};


// We can use WinMain as the entry point for application using
// either ANSI (char) or Unicode (wide) character sets.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CWinApp theApp;

    // Create our view window
    CView myWin;
    myWin.Create();

    // Run the application's message loop
    return theApp.Run();
}
