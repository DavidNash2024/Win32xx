//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CMainWindow class



#include "ThreadApp.h"
#include "MainWnd.h"
#include "Thread.h"
#include "resource.h"
#include <string>
#include <sstream>


CMainWindow::CMainWindow()
{
	// Set the number of threads
	m_nThreads = 20;
}

CMainWindow::~CMainWindow()
{
	// m_pCThreads is a vector of CThread pointers
	// Delete each CThread object
	for (int i = 0 ; i < m_nThreads; i++)
	{
		delete m_pCThreads[i];
	}
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
	// Create each CThread object
	for (int i = 0 ; i < m_nThreads ; i++)
	{
		// Create the thread and store the CThread pointer
		m_pCThreads.push_back(new CThread(i));
	}
}

void CMainWindow::OnAllWindowsCreated()
{
	tStringStream str;
	str << m_nThreads << _T(" Threads with windows created\n");
	TRACE(str.str().c_str());
}

LRESULT CMainWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int nWindowsCreated = 0;

	switch (uMsg)
	{
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_SETFOCUS)
			::SetFocus(m_hWnd);
		break;
	
	case WM_CLOSE:
		{
			//Close the thread windows
			for (int i = 0 ; i < m_nThreads ; i++)
			{
				::SendMessage(m_pCThreads[i]->GetTestWindow().GetHwnd(), WM_CLOSE, 0, 0);
			}
		}
		break;
	
	case WM_DESTROY:	
		// Pause this thread to allow the others to complete
		Sleep(500);
		
		// Post the WM_QUIT message to terminate the primary thread.
		::PostQuitMessage(0);
		break;
	
	case WM_WINDOWCREATED:
		// Message recieved when a test window is created
		if (++nWindowsCreated == m_nThreads)
			OnAllWindowsCreated();
		break;
	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

