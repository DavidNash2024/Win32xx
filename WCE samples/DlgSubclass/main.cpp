///////////////////////////////////////
// main.cpp

#include "DlgSubclassApp.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	TCHAR szBuf[MAX_STRING_SIZE];
	::LoadString(hInstance, IDW_MAIN, szBuf, MAX_STRING_SIZE -1);
	HWND hWnd = FindWindow(szBuf, NULL);

	// If application is already running, restore it and quit
	if (hWnd)
	{
		SetForegroundWindow ((HWND)(((DWORD)hWnd) | 0x01));
		TRACE("Application Already running");
		return -1;
	}

	// Start Win32++
	CDlgSubclassApp theApp;

	// Process the window messages
	return theApp.Run();
}

