//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CMainWindow class

#include "stdafx.h"
#include "MainWnd.h"


CMainWindow::CMainWindow() : m_nWindowsCreated(0)
{
	// Set the number of threads
	m_nTestWin = 20;

	// A couple of notes in case you're tempted to test how many threads with test windows can be created ...

	// Note 1: A Windows limit of 10000 handles per process imposes a practical limit of aprox 1000 test windows.
	//         Refer to: http://support.microsoft.com/kb/327699
	// Note 2: All our threads belong to the one process.
	// Note 3: Creating (or destroying) more than say 200 windows may temporarily stress the Explorer process.
	// Note 4: This sample is intended as "proof of concept" only. A well written program should not require 20 GUI threads!
}

HWND CMainWindow::Create(HWND hParent)
{
	tString str = _T("Main Thread Window");
	
	// Create the main window
	return CreateEx(WS_EX_TOPMOST, NULL, str.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		20 , 50, 400, 300, hParent, NULL);
}

void CMainWindow::OnCreate()
{
	// Create each CMyThread object
	for (int i = 1 ; i <= m_nTestWin ; i++)
	{
		// Create the Test Window and store the CTestWindow pointer
		CTestWindow* pTestWin = new CTestWindow(i);
		
		TCHAR str[80];
		wsprintf(str, _T("Thread %d started\n"), i);
		TRACE(str);

		m_vTestWnd.push_back(pTestWin);
	}
	
	std::vector<TestWndPtr>::iterator iter;
	for (iter = m_vTestWnd.begin(); iter < m_vTestWnd.end(); ++iter)
	{
		(*iter)->ResumeThread();
	}	
}

void CMainWindow::OnAllWindowsCreated()
{
	TCHAR str[80];
	wsprintf(str, _T("%d Test windows created in seperate threads\n"), m_nTestWin);
	TRACE(str);
}

LRESULT CMainWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_SETFOCUS)
			::SetFocus(m_hWnd);
		break;
	
	case WM_CLOSE:
		{
			// Close each thread window. 
			// The thread is then terminated with a WM_QUIT when its window is destroyed.
			std::vector<TestWndPtr>::iterator iter;
			for (iter = m_vTestWnd.begin(); iter < m_vTestWnd.end(); ++iter)
			{
				::SendMessage((*iter)->GetHwnd(), WM_CLOSE, 0, 0);
			} 
		}
		break;
	
	case WM_DESTROY:	
		{	
			// Terminate the primary thread.
			::PostQuitMessage(0); 
		}
		break;
	
	case WM_WINDOWCREATED:
		{
			// Message recieved when a test window is created
			TCHAR str[80];
			++m_nWindowsCreated;
			wsprintf(str, _T("Created Window %d\n"), m_nWindowsCreated);
			TRACE(str);
			if (m_nWindowsCreated == m_nTestWin)
				OnAllWindowsCreated();
		}
		break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

