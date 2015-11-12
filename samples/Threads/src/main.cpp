///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "ThreadApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CThreadApp theApp;

		// Run the application
		return theApp.Run();
	}
	
	// catch all CException types
	catch (const CException &e)
	{
		// Display the exception and quit
		MessageBox(NULL, e.GetText(), A2T(e.what()), MB_ICONERROR);

		return -1;
	}
}


