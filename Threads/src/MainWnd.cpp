//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CMainWindow class

#include "stdafx.h"
#include "ThreadApp.h"
#include "MainWnd.h"
#include "Thread.h"


CMainWindow::CMainWindow()
{
	// Set the number of threads
	m_nThreads = 20;

	// A couple of notes in case you're tempted to test how many threads with test windows can be created ...

	// Note 1: A Windows limit of 10000 handles per process imposes a practical limit of aprox 1000 test windows.
	//         Refer to: http://support.microsoft.com/kb/327699
	// Note 2: All our threads belong to the one process.
	// Note 3: Creating (or destroying) more than say 200 windows may temporarily stress the Explorer process.
	// Note 4: This sample is intended as "proof of concept" only. A well written program should not require 20 GUI threads!
}

CMainWindow::~CMainWindow()
{
	// m_pCThreads is a vector of CThread pointers
	// Delete each CThread object
	std::vector<CThread*>::iterator iter;
	for (iter = m_pCThreads.begin(); iter < m_pCThreads.end(); ++iter)
	{
		delete (*iter);
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
	try
	{
		// Create each CThread object
		for (int i = 0 ; i < m_nThreads ; i++)
		{
			// Create the thread and store the CThread pointer
			CThread* pThread = new CThread(i);
			
			TCHAR str[80];
			wsprintf(str, _T("Thread %d started\n"), i + 1);
			TRACE(str);

			m_pCThreads.push_back(pThread);
		}
	
		std::vector<CThread*>::iterator iter;
		for (iter = m_pCThreads.begin(); iter < m_pCThreads.end(); ++iter)
		{
			(*iter)->Start();
		}
	}

	catch (const CWinException &e)
	{
		e.Output();
		DebugErrMsg(_T("Exception in CMainWindow::OnCreate"));
	}

	catch (...)
	{
		DebugErrMsg(_T("Exception in CMainWindow::OnCreate"));
	}
}

void CMainWindow::OnAllWindowsCreated()
{
	TCHAR str[80];
	wsprintf(str, _T("%d Test windows created in seperate threads\n"), m_nThreads);
	TRACE(str);
}

LRESULT CMainWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			std::vector<CThread*>::iterator iter;
			for (iter = m_pCThreads.begin(); iter < m_pCThreads.end(); ++iter)
			{
				::SendMessage((*iter)->GetTestWindow().GetHwnd(), WM_CLOSE, 0, 0);
			}
		}
		break;
	
	case WM_DESTROY:	
		{
			std::vector<CThread*>::iterator iter;
			for (iter = m_pCThreads.begin(); iter < m_pCThreads.end(); ++iter)
			{
				PostThreadMessage((*iter)->GetThreadID(), WM_QUIT,0,0);
				
				// Pause this thread and wait for the specified thread to end
				::WaitForSingleObject((*iter)->GetHandle(), 100);
			}
		
			// Post the WM_QUIT message to terminate the primary thread.
			::PostQuitMessage(0);
		}
		break;
	
	case WM_WINDOWCREATED:
		{
			// Message recieved when a test window is created
			TCHAR str[80];
			wsprintf(str, _T("Created Window %d\n"), ++nWindowsCreated);
			TRACE(str);
			if (nWindowsCreated == m_nThreads)
				OnAllWindowsCreated();
		}
		break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

