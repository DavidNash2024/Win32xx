///////////////////////////////////////////////
// TextApp.cpp -  Definitions for the CTextApp class

#include "stdafx.h"
#include "TextApp.h"
#include "resource.h"


CTextApp::CTextApp()
{
}

BOOL CTextApp::InitInstance()
{
	//Create the Frame Window
	m_Frame.Create();	// throws a CWinException on failure  

	return TRUE;
}

