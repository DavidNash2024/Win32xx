//--------------------------------------------------------------------------------------
// File: main.cpp
//
// This application displays a 3D cube using Direct3D 11
//
//--------------------------------------------------------------------------------------

#include "wxx_wincore.h"

#include "DXApp.h"
#include "DXView.h"
#include "resource.h"



//--------------------------------------------------------------------------------------
// Entry point to the program. Starts the Win32++ framework and runs the message loop.
//--------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	CDXApp theApp;
	theApp.Run();

	return 0;
}



