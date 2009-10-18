//////////////////////////////////////////////
//SdiApp.cpp


#include "stdafx.h"
#include "FormApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CFormApp theApp;

	// Run the application
	return theApp.Run();
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

