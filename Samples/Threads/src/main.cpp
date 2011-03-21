///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "ThreadApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CThreadApp theApp;

	// Run the application
	theApp.Run();
	
	return 0;
}


