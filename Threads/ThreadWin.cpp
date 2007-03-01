//////////////////////////////////////////////
// ThreadWin.cpp
//  Definitions for the CThreadWnd class

#include "ThreadApp.h"
#include "ThreadWin.h"


CThreadWnd::CThreadWnd()
{
}

void CThreadWnd::CreateWin(int i)
{
	TCHAR str[80];

	m_iNum = i + 1;
	::wsprintf(str, TEXT("Thread #%d"), m_iNum);
	CreateEx(0L, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		320, 50 + i, 300, 200, NULL, NULL);
}

void CThreadWnd::OnInitialUpdate()
{
	// Send a message to MainWnd when the window is created
	CMainWnd& MainWnd = ((CThreadApp*)GetApp())->GetMainWnd();
	::SendMessage(MainWnd.GetHwnd(), WM_WINDOWCREATED, 0, 0);
}

LRESULT CThreadWnd::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static LRESULT nMessages = 0;
	switch (uMsg)
	{	
	case WM_CLOSE:
		{
			TCHAR str[80];
			::wsprintf(str, TEXT("Closing thread #%d"), m_iNum);
			TRACE(str);
		}
		break;

	case WM_DESTROY:
		// Post the WM_QUIT message to terminate the thread.
		::PostQuitMessage(0);
		return 0L;
	
	case WM_TESTMESSAGE:
		// return the number of these messages processsed so far
		return ++nMessages;
	}

	//Use the CWnd default message handling for remaining messages
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}

