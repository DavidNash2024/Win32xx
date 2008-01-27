///////////////////////////////////////////////
//SubApp.cpp -  Definitions for the CSubApp class


#include "SubApp.h"


CSubApp::CSubApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CSubApp::~CSubApp()
{
}

BOOL CSubApp::InitInstance()
{
	TRACE("Move the mouse over the button to see messages\n");

	//Create the Outer Window
	m_Win.Create();

	return TRUE;
}
