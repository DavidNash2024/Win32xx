//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CMainWindow class

#include "stdafx.h"
#include "MainWnd.h"


CMainWindow::CMainWindow() : m_nWindowsCreated(0)
{
	// Set the number of threads
	m_nTestWnd = 20;

	// A couple of notes in case you're tempted to test how many threads with test windows can be created ...

	// Note 1: Each process has a limited amount of resources allocated to it. Attempts to create
	//         more than say 1000 threads in a process might fail and throw a CWinException.
	// Note 2: All our threads belong to the one process.
	// Note 3: Creating (or destroying) more than say 200 windows may temporarily stress the Explorer process.
	// Note 4: This sample is intended as "proof of concept" only. A well written program should not require 20 GUI threads!
}

void CMainWindow::AppendText(LPCTSTR szStr)
{
	// This function appends text to an edit control

	// Append Line Feed
	int ndx = (int)m_EditWnd.SendMessage(WM_GETTEXTLENGTH, 0, 0);
	if (ndx)
	{
		m_EditWnd.SendMessage(EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
		m_EditWnd.SendMessage(EM_REPLACESEL, 0, (LPARAM) (_T("\r\n")));
	}


	// Append text
	ndx = (int)m_EditWnd.SendMessage(WM_GETTEXTLENGTH, 0, 0);
	m_EditWnd.SendMessage(EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
	m_EditWnd.SendMessage(EM_REPLACESEL, 0, (LPARAM) szStr);

	// Also send output to the debugger for display
	TRACE(szStr);
	TRACE(_T("\n"));
}

HWND CMainWindow::Create(HWND hParent)
{
	CString str = _T("Main Thread Window");

	// Create the main window
	CRect rc(20 , 50, 400, 500);
	return CreateEx(WS_EX_TOPMOST, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		rc, hParent, 0);
}

int CMainWindow::OnCreate(LPCREATESTRUCT pcs)
{
	UNREFERENCED_PARAMETER(pcs);

	// Create the Edit child window to display text
//	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL;
//	m_EditWnd.CreateEx(0, _T("Edit"), NULL, dwStyle, CRect(0,0,0,0), *this, 0);
	m_EditWnd.Create(*this);
//	m_EditWnd.SetWindowLongPtr(GWL_STYLE, dwStyle);

	// Permit the Edit window to display up to 128000 characters
//	m_EditWnd.SendMessage(EM_SETLIMITTEXT, 128000, 0);

	// Set the font for the Edit window
//	m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
//		            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
//  	m_EditWnd.SetFont(m_Font, FALSE);

	// Create each CMyThread object
	for (int i = 1 ; i <= m_nTestWnd ; i++)
	{
		CMyThread* pMyThread = new CMyThread(i);
		m_vMyThread.push_back(pMyThread);
	}

	// Create the threads belonging to the MyThread objects
	// Each thread creates a TestWindow when it runs 
	std::vector<MyThreadPtr>::iterator iter;
	for (iter = m_vMyThread.begin(); iter < m_vMyThread.end(); ++iter)
	{
		try
		{
			(*iter)->CreateThread(CREATE_SUSPENDED);
			(*iter)->ResumeThread();

			CString str;
			str.Format( _T("Thread %d started "), (*iter)->GetThreadCount() );
			AppendText(str);
		}
			
		catch (CWinException &e)
		{
			// Display the exception and allow the program to continue
			CString Error = CString(e.GetText()) + "\n" + CString(e.GetErrorString());
			::MessageBox(NULL, Error, A2T(e.what()), MB_ICONERROR) ;

			// return -1 here to close the main window and end the application
		//	return -1;
		}
	}

	return 0;
}

void CMainWindow::OnClose()
{
	// Close each thread window.
	// The thread is then terminated with PostWuitMessage when its window is destroyed.

	std::vector<MyThreadPtr>::iterator iter;
	for (iter = m_vMyThread.begin(); iter < m_vMyThread.end(); ++iter)
	{
		if ((*iter)->GetTestWnd()->IsWindow())
			(*iter)->GetTestWnd()->SendMessage(WM_CLOSE, 0, 0);
	}

	Destroy();
}

void CMainWindow::OnDestroy()
{
	// End the application
	::PostQuitMessage(0);
}

void CMainWindow::OnAllWindowsCreated()
{
	CString str;
	str.Format( _T("%d Test windows created in seperate threads"), m_nTestWnd );
	AppendText(str);
}

void CMainWindow::OnWindowCreated()
{
	// Message recieved when a test window is created
	CString str;
	++m_nWindowsCreated;
	str.Format( _T("Created Window %d"), m_nWindowsCreated );
	if (m_nWindowsCreated >= 455)
		TRACE("");
	AppendText(str);

	if (m_nWindowsCreated == m_nTestWnd)
		OnAllWindowsCreated();
}

LRESULT CMainWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_WINDOWCREATED:
		OnWindowCreated();
		break;
	case WM_SIZE:
		m_EditWnd.SetWindowPos(0, GetClientRect(), 0);
		break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

