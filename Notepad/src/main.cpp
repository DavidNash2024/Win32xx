///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "TextApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CTextApp theApp;

	// Run the application
	return theApp.Run();
}


