///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "ReBarApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CReBarApp theApp;

		// Run the application and the message loop
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

