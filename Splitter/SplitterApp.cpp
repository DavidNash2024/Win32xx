///////////////////////////////////////////////////////////
//SplitterApp.cpp -  Definitions for the CSplitterApp class


#include "resource.h"
#include "SplitterApp.h"


CSplitterApp::CSplitterApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Open the Trace Window
	TRACE(TEXT("Trace started"));

	//Create the Frame Window
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (!m_Frame.GetHwnd())
	{
		::MessageBox(NULL, TEXT("Failed to create Frame window"), TEXT("ERROR"), MB_ICONERROR);
		::PostQuitMessage(0);
	}
}



