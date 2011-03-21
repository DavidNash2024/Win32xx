///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "DialogApp.h"
#include "resource.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CDialogApp theApp;

	// Run the application
	return theApp.Run();
}

