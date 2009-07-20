//////////////////////////////////////////////
// TestWnd.cpp
//  Definitions for the CTestWindow class



#include "TestWnd.h"
#include "ThreadApp.h"


CTestWindow::CTestWindow()
{
}

void CTestWindow::CreateWin(int i)
{
	tStringStream str;

	m_nThread = i + 1;
	str << _T("Thread ") << m_nThread;
	CreateEx(0L, NULL, str.str().c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		70 + 20*i, 120 + 20*i, 300, 200, NULL, NULL);
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
	static LRESULT nMessages = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
		{
			tStringStream str;
			str << _T("Closing test Window ") << m_nThread << _T("\n");
			TRACE(str.str().c_str());
		}
		break;

	case WM_DESTROY:
		// Post the WM_QUIT message to terminate the thread.
		::PostQuitMessage(0);
		break;

	case WM_TESTMESSAGE:
		// return the number of WM_TESTMESSAGE messages processsed by this thread so far
		return ++nMessages;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

