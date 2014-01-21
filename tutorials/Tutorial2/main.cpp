///////////////////////////////////
// main.cpp

// Note:  
//  * Add the Win32++\include  directory to project's additional include directories

#include "wincore.h"


// A class that inherits from CWnd. It is used to create the window.
class CView : public CWnd
{
public:
	CView() {}
	virtual void OnDestroy() { PostQuitMessage(0); } // Ends the program
	virtual ~CView() {}
};


// A class that inherits from CWinApp. 
// It is used to run the application's message loop.
class CSimpleApp : public CWinApp
{
public:
	CSimpleApp() {}
    virtual ~CSimpleApp() {}
	virtual BOOL InitInstance();

private:
    CView m_View;
};

BOOL CSimpleApp::InitInstance()
{
    // Create the Window
    m_View.Create();

	return TRUE;
}


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CSimpleApp MyApp;

	// Run the application
    return MyApp.Run();
}
