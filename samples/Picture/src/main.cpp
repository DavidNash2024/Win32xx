///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "PictureApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		// Start Win32++
		CPictureApp theApp;

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
