///////////////////////////////////
// main.cpp

// The directory of the include directory should also be added to project's 
//  additional include directories.

#include "wincore.h"


class CView : public CWnd
{
public:
	CView() {}
	virtual ~CView() {}
};


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

// Note:  This application doesn't end when the window is closed

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CSimpleApp MyApp;

	// Run the application
    return MyApp.Run();
}
