///////////////////////////////////////
// main.cpp

#include "wincore.h"

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
	CWinApp MyApp;

	// Create a CMyWindow object
	CMyWindow MyWindow;

	// Create (and display) the window
	MyWindow.Create();

	// Run the application's message loop
	return MyApp.Run();
}


