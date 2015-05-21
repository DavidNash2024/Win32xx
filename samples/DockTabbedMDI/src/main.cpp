///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "TabbedMDIApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CTabbedMDIApp theApp;

		// Run the application
		return theApp.Run();
	}
	
	// catch CWinException exceptions
	catch (CWinException &e)
	{
		// Display the exception and quit
		CString Error = CString(e.GetText()) + "\n" + CString(e.GetErrorString());
		MessageBox(NULL, Error, _T("CWinException thrown"), MB_ICONERROR) ;

		return -1;
	}
}
