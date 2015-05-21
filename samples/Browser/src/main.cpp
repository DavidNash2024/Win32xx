/////////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "BrowserApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CBrowserApp theApp;

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





