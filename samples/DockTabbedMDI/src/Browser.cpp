//////////////////////////////////////////////////////
// Browser.cpp -  Definitions for the CViewWeb class

#include "stdafx.h"
#include "Browser.h"
#include "resource.h"



///////////////////////////////////////////////
// CViewWeb functions
void CViewWeb::OnInitialUpdate()
{
	// Load the Win32++ home page
	Navigate2(_T("http://sourceforge.net/projects/win32-framework/"));
}