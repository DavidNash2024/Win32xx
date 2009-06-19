//////////////////////////////////////////////
//SdiApp.cpp

#include "stdafx.h"
#include "SdiApp.h"

INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CSdiApp theApp;

	// Run the application
	return theApp.Run();
}



// Definitions for the CSdiApp class
CSdiApp::CSdiApp()
{
}

CSdiApp::~CSdiApp()
{
}

BOOL CSdiApp::InitInstance()
{
	//Create the Frame Window
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (!m_Frame.GetHwnd())
	{
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

