//////////////////////////////////////////////
// MainWin.cpp
//  Definitions for the CThreadWnd class


#include "ThreadApp.h"
#include "MainWin.h"


extern HANDLE g_ThreadHandles[MAX_THREADS];
extern CThreadWnd* g_ThreadCWnds[MAX_THREADS];

DWORD WINAPI ThreadCallback(LPVOID pInt)
{
	int i = *((int*)pInt);
	TCHAR str[80];
	wsprintf(str, TEXT("Thread #%d started"), i + 1);
	TRACE(str);

	g_ThreadCWnds[i] = new CThreadWnd;
	g_ThreadCWnds[i]->CreateWin(i);

	//Each thread with a window has its own message loop
	return GetApp()->MessageLoop();
}

CMainWin::CMainWin()
{
}

CMainWin::~CMainWin()
{
}

void CMainWin::Create()
{
	TCHAR str[80];
	wsprintf(str, TEXT("Main Thread Window"));
	CreateEx(0L, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		50 , 100, 500, 400, NULL, NULL);
}

void CMainWin::OnCreate()
{
	for (int i = 0 ; i < MAX_THREADS ; i++)
	{
		m_IntArray[i] = i;
		g_ThreadHandles[i] = CreateThread(NULL, 0, ThreadCallback, (LPVOID) &m_IntArray[i], 0, &m_ThreadID[i]);
	}
}

LRESULT CMainWin::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		{
			//Close the thread windows
			for (int i = 0 ; i < MAX_THREADS ; i++)
				::SendMessage(g_ThreadCWnds[i]->GetHwnd(), WM_CLOSE, 0, 0);
		}
		break;

	case WM_DESTROY:
		// Post the WM_QUIT message to terminate the thread.
		::PostQuitMessage(0);
		return 0;
	}

	//Use the CWnd default message handling for remaining messages
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}

