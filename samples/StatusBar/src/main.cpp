///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "StatusBarApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CStatusBarApp theApp;

		// Run the application and the message loop
		return theApp.Run();
	}

	// catch all unhandled CException types
	catch (const CException &e)
	{
		// Display the exception and quit
		MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

		return -1;
	}
}

