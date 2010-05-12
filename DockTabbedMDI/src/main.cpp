///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "TabbedMDIApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CTabbedMDIApp theApp;

	CWnd* pWin = new CWnd;

	// Run the application
	return theApp.Run();
}


