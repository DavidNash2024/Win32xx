//////////////////////////////////////////////
// TestWnd.cpp
//  Definitions for the CTestWindow class

#include "stdafx.h"
#include "TestWnd.h"
#include "ThreadApp.h"


CTestWindow::CTestWindow(int nWindow) : m_nWindow(nWindow)
{
}

HWND CTestWindow::Create(CWnd* pParent)
{
	UNREFERENCED_PARAMETER(pParent);

	CString Title;
	Title.Format( _T("Test Window %d"), m_nWindow );
	return CreateEx(0L, NULL, Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		70 + 20*m_nWindow, 120 + 20*m_nWindow, 300, 200, NULL, NULL);
}

BOOL CTestWindow::InitInstance()
{
	// This function runs when the thread starts

	// Create a test window for this thread
	Create();

	return TRUE;	// return TRUE to run the message loop
}

void CTestWindow::OnClose()
{
	CString str;
	str.Format( _T("Closing test Window %d\n"), m_nWindow );
	TRACE(str);

	Destroy();
}
void CTestWindow::OnDestroy()
{
	// Terminate the thread.
	::PostQuitMessage(0);
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


