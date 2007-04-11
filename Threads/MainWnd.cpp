//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CThreadWnd class


#include "ThreadApp.h"
#include "MainWnd.h"
#include "Thread.h"
#include "resource.h"


CMainWindow::CMainWindow() : m_pCThreads(NULL)
{
	// Set the number of threads
	m_nThreads = 20;
}

CMainWindow::~CMainWindow()
{
	for (int i = 0 ; i < m_nThreads; i++)
		delete m_pCThreads[i];

	delete []m_pCThreads;
}

void CMainWindow::Create()
{
	TCHAR str[80];
	wsprintf(str, TEXT("Main Thread Window"));
	
	// Create the main window
	CreateEx(WS_EX_TOPMOST, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		20 , 50, 400, 300, NULL, NULL);
}

void CMainWindow::CreateThreads()
{
	m_pCThreads = new CThread*[m_nThreads];

	for (int i = 0 ; i < m_nThreads ; i++)
	{
		// Create the thread and store the CThread pointer
		m_pCThreads[i] = new CThread(i);
	}
}

void CMainWindow::OnCreate()
{
	CreateThreads();
}

void CMainWindow::OnAllWindowsCreated()
{
	TCHAR str[80];
	wsprintf(str, TEXT("%d  Windows Created"), m_nThreads);	
	TRACE(str);
}

LRESULT CMainWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				::SendMessage(m_pCThreads[i]->GetTestWindow()->GetHwnd(), WM_CLOSE, 0, 0);
		}
		break;
	
	case WM_DESTROY:	
		// Post the WM_QUIT message to terminate the primary thread.
		::PostQuitMessage(0);
		return 0L;
	
	case WM_WINDOWCREATED:
		// Message recieved when a test window is created
		if (++nWindowsCreated == m_nThreads)
			OnAllWindowsCreated();
		return 0L;
	}

	//Use the CWnd default message handling for remaining messages
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}

