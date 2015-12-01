///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "DXApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CDXApp MyApp;

		// Run the application
		return MyApp.Run();
	}
	
	// catch all unhandled CException types
	catch (const CException &e)
	{
		// Display the exception and quit
		MessageBox(NULL, e.GetText(), A2T(e.what()), MB_ICONERROR);

		return -1;
	}
}
