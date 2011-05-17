///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "DialogApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CDialogApp theApp;

		// Run the application
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

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CDialogApp theApp;

	// Run the application
	return theApp.Run();
}

