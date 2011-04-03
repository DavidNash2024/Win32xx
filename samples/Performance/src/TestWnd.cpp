//////////////////////////////////////////////
// TestWnd.cpp
//  Definitions for the CTestWindow class

#include "stdafx.h"
#include "TestWnd.h"
#include "PerfApp.h"


CTestWindow::CTestWindow()
{
}

void CTestWindow::CreateWin(int i)
{
	m_nWindow = i + 1;
	TCHAR str[80];
	wsprintf(str, _T("Test Window %d"), m_nWindow);
	CreateEx(0L, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		420, 50 + i, 300, 200, NULL, NULL);
}

void CTestWindow::OnInitialUpdate()
{
	// Get a reference to the CMainWnd object
	CMainWindow& MainWnd = ((CPerformanceApp*)GetApp())->GetMainWnd();

	// Post a message to MainWnd when the window is created. 
	::PostMessage(MainWnd.GetHwnd(), WM_WINDOWCREATED, 0, 0);
}

LRESULT CTestWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static LRESULT nMessages = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
		{
			TCHAR str[80];
			wsprintf(str, _T("Closing test Window #%d\n"), m_nWindow);
			TRACE(str);
		}
		break;

	case WM_TESTMESSAGE:
		// return the number of WM_TESTMESSAGE messages processsed so far
		return ++nMessages;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

