//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CThreadWnd class


#include "ThreadApp.h"
#include "MainWnd.h"
#include "Thread.h"


CMainWindow::CMainWindow()
{
}

CMainWindow::~CMainWindow()
{
	for (int i = 0 ; i < MAX_THREADS; i++)
		delete m_pCThreads[i];
}

void CMainWindow::Create()
{
	TCHAR str[80];
	wsprintf(str, TEXT("Main Thread Window"));
	CreateEx(WS_EX_TOPMOST, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		20 , 50, 300, 300, NULL, NULL);
}

void CMainWindow::OnCreate()
{
	RECT r;
	::GetClientRect(m_hWnd, &r);
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;
	m_hEdit = ::CreateWindowEx(0L, TEXT("Edit"), TEXT(""), dwStyle, r.left, r.top, r.right - r.left, r.bottom - r.top,
						m_hWnd, NULL, GetApp()->GetInstanceHandle(), NULL);
}

void CMainWindow::OnInitialUpdate()
{
	for (int i = 0 ; i < MAX_THREADS ; i++)
	{
		m_iNums[i] = i;
		m_pCThreads[i] = new CThread(&m_iNums[i]);
	}
}

void CMainWindow::OnSize()
{
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::MoveWindow(m_hEdit, 0, 0, r.right - r.left, r.bottom - r.top, TRUE);
}

void CMainWindow::OnAllWindowsCreated()
{
	SendText(TEXT("All Windows Created"));
	SendText(TEXT("Ready to run performance test"));

	int nRet = IDOK;
	while(nRet == IDOK)
	{
		nRet = MessageBox(m_hWnd, TEXT("Click OK to begin"), TEXT("Performance Test"), MB_ICONINFORMATION|MB_OKCANCEL);
		if (nRet == IDOK)
			PerformanceTest();
	}
}

void CMainWindow::PerformanceTest()
{
	TCHAR str[80];
	LRESULT lr = 0;

	SendText(TEXT("Sending 100,000 Messages"));

	int nMessages = 0;
	HWND hWnd = m_pCThreads[(MAX_THREADS-1)/2]->m_pTestWindow->GetHwnd();
	DWORD tStart = ::GetTickCount();

	while(nMessages++ < 100000)
        lr = ::SendMessage(hWnd, WM_TESTMESSAGE, 0, 0);

	DWORD tEnd = ::GetTickCount();
	DWORD mSeconds = tEnd - tStart;

	wsprintf(str, "%d  milliseconds to process 100 thousand messages", mSeconds);
	SendText(str);
	MessageBox(NULL, str, TEXT("Info"), MB_OK);

	wsprintf(str, "%d total messages sent ", lr);
	TRACE(str);  
}

void CMainWindow::SendText(LPCTSTR str)
{
	::SendMessage(m_hEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)str);
	::SendMessage(m_hEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)TEXT("\r\n"));
	::SendMessage(m_hEdit, EM_SCROLLCARET, (WPARAM)0, (LPARAM)0);

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
			for (int i = 0 ; i < MAX_THREADS ; i++)
				::SendMessage(m_pCThreads[i]->m_pTestWindow->GetHwnd(), WM_CLOSE, 0, 0);
		}
		break;
	case WM_DESTROY:	
		// Post the WM_QUIT message to terminate the primary thread.
		::PostQuitMessage(0);
		return 0L;
	case WM_SIZE:
		OnSize();
		return 0;
	case WM_WINDOWCREATED:
		if (++nWindowsCreated == MAX_THREADS)
			OnAllWindowsCreated();
		return 0L;
	}

	//Use the CWnd default message handling for remaining messages
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}

