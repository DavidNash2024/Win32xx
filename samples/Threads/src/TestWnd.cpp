//////////////////////////////////////////////
// TestWnd.cpp
//  Definitions for the CTestWindow class

#include "stdafx.h"
#include "TestWnd.h"
#include "ThreadApp.h"


CTestWindow::CTestWindow(int nWindow) : m_nWindow(nWindow)
{
}

HWND CTestWindow::Create(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);

	TCHAR szTitle[80];
	wsprintf(szTitle, _T("Test Window %d"), m_nWindow);
	return CreateEx(0L, NULL, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		70 + 20*m_nWindow, 120 + 20*m_nWindow, 300, 200, NULL, NULL);
}

BOOL CTestWindow::InitInstance()
{
	// This function runs when the thread starts

	// Create a test window for this thread
	Create();

	return TRUE;	// return TRUE to run the message loop
}

void CTestWindow::OnInitialUpdate()
{
	// Get a reference to the CMainWnd object
	CMainWindow& MainWnd = ((CThreadApp*)GetApp())->GetMainWnd();

	// Post a message to MainWnd when the window is created. The MainWnd window
	//  is in a different thread, so PostMessage is preferred over SendMessage.
	//  SendMessage would wait for the MainWnd thread to respond.
	::PostMessage(MainWnd.GetHwnd(), WM_WINDOWCREATED, 0, 0);
}

LRESULT CTestWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		{
			TCHAR str[80];
			wsprintf(str, _T("Closing test Window %d\n"), m_nWindow);
			TRACE(str);
		}
		break;

	case WM_DESTROY:
		// Terminate the thread.
		::PostQuitMessage(0);
		break;

	}

	return WndProcDefault(uMsg, wParam, lParam);
}

