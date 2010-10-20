/////////////////////////////////////////
// main.cpp

#include "stdafx.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Displays a dialog contained in MyDLL.dll

	// Load the MyDLL.dll
	HMODULE hModule = LoadLibrary(_T("MyDLL.dll"));
	assert(hModule);
	
	// Create a pointer to MyDLL's ShowDialog function
	typedef void WINAPI SHOWDIALOG();
	SHOWDIALOG* pfnShowDialog = (SHOWDIALOG*)::GetProcAddress(hModule, "ShowDialog");
	assert(pfnShowDialog);
	
	// Call MyDLL's ShowDialog function
	pfnShowDialog();

	// We get here when the dialog is closed
	FreeLibrary(hModule);

	return 0;
}



