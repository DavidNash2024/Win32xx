//////////////////////////////////////////////
// FormApp.cpp

#include "stdafx.h"
#include "App.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CApp theApp;

		// Run the application
		return theApp.Run();
	}
	
	// catch all unhandled CException types
	catch (const CException &e)
	{
		// Display the exception and quit
		MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

		return -1;
	}
}


// Definitions for the CApp class
CApp::CApp()
{
}

CApp::~CApp()
{
}

BOOL CApp::InitInstance()
{
	//Create the Frame Window
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (m_Frame.GetHwnd() == 0)
	{
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

