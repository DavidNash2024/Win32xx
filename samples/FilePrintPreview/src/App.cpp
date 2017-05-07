/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.CPP) *
********************************************************************************

	Implementation of the CApp class for the FilePrintPreview demo program
	and the WinMain interface with the Windows operating system.

********************************************************************************

	Acknowledgment. This demo program was adapted from the PrintPreview 
	sample program appearing in the Win32++ framework sample folder, created 
	by  David Nash and published under the permissions granted in that work.
	The adaptation here implements the PrintView window as a separate popup
	window that appears on screen along with the regular program window.
	This work has been developed under the co-authorship of Robert C. 
	Tausworthe and David Nash, and released under the copyright provisions 
	of the Win32++ Interface Classes software, copyright (c) David Nash,
	2005-2017. The former author acknowledges and thanks the latter for his 
	patient direction and inspiration in the development of the classes of 
	these classes.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

/*============================================================================*/
	BOOL CApp::
InitInstance()                                                          /*

	Called by the framework to create the frame
*-----------------------------------------------------------------------------*/
{
	//Create the Frame Window
	if (m_Frame.Create() == 0)
	{
		// the Frame creation fails
		::MessageBox(NULL, _T("Failed to create Frame window"),
		     _T("ERROR"), MB_ICONERROR);
		return FALSE; // returning FALSE ends the application
	}
	return TRUE;
}

/*******************************************************************************

	Windows API top-level program

*=============================================================================*/
	int APIENTRY 
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)				/*

	Launch the application.
*-----------------------------------------------------------------------------*/
{
	try
	{
		  // Start Win32++
		CApp theApp;

		  // Run the application: invoke theApp::InitInstance() and
		  // start the thread's message loop
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

