//////////////////////////////////////////////
// ThreadWin.cpp
//  Definitions for the CThreadWnd class

#include "ThreadWin.h"


CThreadWnd::CThreadWnd()
{
}

void CThreadWnd::CreateWin(int i)
{
	m_iNum = i + 1;
	TCHAR str[80];
	wsprintf(str, TEXT("Thread #%d"), m_iNum);
	CreateEx(0L, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		70 + 20*i, 120 + 20*i, 300, 200, NULL, NULL);
}

LRESULT CThreadWnd::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		{
			TCHAR str[80];
			wsprintf(str, TEXT("Closing thread #%d"), m_iNum);
			TRACE(str);
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

