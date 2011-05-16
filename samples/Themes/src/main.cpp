///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "ThemesApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CThemesApp theApp;

		// Run the application
		return theApp.Run();
	}
	
	catch (std::exception &e)
	{
		// Process the exception and quit 
		e.what();
		return -1;
	}
}

