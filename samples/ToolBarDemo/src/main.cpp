///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "FrameApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CToolBarDemoApp theApp;

		// Run the application and the message loop
		return theApp.Run();
	}
	
	// catch all exceptions inherited from std::exception
	catch (std::exception &e)
	{
		// Process the exception and quit 
		e.what();
		return -1;
	}
}

