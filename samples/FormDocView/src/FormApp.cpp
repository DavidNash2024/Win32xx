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
	
	// catch CWinException exceptions
	catch (CWinException &e)
	{
		// Process the exception and quit
		CString Error = CString(e.GetText()) + "\n" + CString(e.GetErrorString());
		MessageBox(NULL, Error, _T("CWinException thrown"), MB_ICONERROR) ;

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
	if (m_Frame.GetHwnd() == 0)
	{
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

