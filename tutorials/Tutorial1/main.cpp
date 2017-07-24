///////////////////////////////////////
// main.cpp

#include "wxx_wincore.h"

// Note:  
//  * Add the Win32++\include  directory to project's additional include directories


// A class that inherits from CWnd. It is used to create the window.
class CMyWindow : public CWnd
{
public:
	CMyWindow() {}
	virtual void OnDestroy() { PostQuitMessage(0); }	// Ends the program
	virtual ~CMyWindow() {}
};


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CWinApp theApp;

	// Create a CMyWindow object
	CMyWindow myWindow;

	// Create (and display) the window
	myWindow.Create();

	// Run the application's message loop
	return theApp.Run();
}


