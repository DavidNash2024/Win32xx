//////////////////////////////////////////////
// FormApp.cpp

#include "stdafx.h"
#include "FormApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CFormApp theApp;

		// Run the application
		return theApp.Run();
	}
	
	// catch all exceptions inherited from std::exception
	catch (std::exception &e)
	{
		// Process the exception and quit 
		e.what();
		return -1;
	}
}


// Definitions for the CFormApp class
CFormApp::CFormApp()
{
}

CFormApp::~CFormApp()
{
}

BOOL CFormApp::InitInstance()
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

