///////////////////////////////////
// main.cpp

// Note:
//  * Add the Win32++\include  directory to project's additional include directories

#include "wxx_wincore.h"


//////////////////////////////////////////
// CView is the application's main window.
class CView : public CWnd
{
public:
    CView() {}
    virtual void OnDestroy() { PostQuitMessage(0); } // Ends the program
    virtual ~CView() {}
};


////////////////////////////////////////////////////
// A class that inherits from CWinApp.
// It is used to run the application's message loop.
class CSimpleApp : public CWinApp
{
public:
    CSimpleApp() {}
    virtual ~CSimpleApp() {}
    virtual BOOL InitInstance();

private:
    CView m_view;
};

// Called when the application starts.
BOOL CSimpleApp::InitInstance()
{
    // Create the Window
    m_view.Create();

    return TRUE;
}


// WinMain is the program's entry point. The program starts here.
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CSimpleApp theApp;

    // Run the application
    return theApp.Run();
}
