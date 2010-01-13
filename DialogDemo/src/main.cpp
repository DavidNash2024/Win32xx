///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "DialogDemoApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CDlgSubclassApp theApp;

	// Run the application
	return theApp.Run();
}

