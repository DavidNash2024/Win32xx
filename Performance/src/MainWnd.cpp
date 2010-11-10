//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CMainWindow class

#include "stdafx.h"
#include "PerfApp.h"
#include "MainWnd.h"
#include "resource.h"


CMainWindow::CMainWindow() : m_hEdit(NULL), m_hFont(NULL), m_nTestMessages(0), m_nTestWindows(0)
{
}

CMainWindow::~CMainWindow()
{
	if (m_hFont)
		::DeleteObject(m_hFont);
}

HWND CMainWindow::Create(HWND hParent /*= 0*/)
{
	tString str = _T("Main Window");

	// Create the main window
	return CreateEx(WS_EX_TOPMOST, NULL, str.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		20 , 50, 400, 300, hParent, NULL);
}

void CMainWindow::CreateTestWindows(int nWindows)
{
	m_nTestWindows = nWindows;

	for (int i = 0 ; i < nWindows ; i++)
	{
		// Create the test windows
		TestWindowPtr pTestWindow = new CTestWindow();
		m_pCTestWindows.push_back(pTestWindow);
		m_pCTestWindows[i]->CreateWin(i);
	}
}

void CMainWindow::OnCreate()
{
	CRect r = GetClientRect();
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

	// Create an Edit window over the client area of the main window
	m_hEdit = ::CreateWindowEx(0L, _T("Edit"), _T(""), dwStyle, r.left, r.top, r.right - r.left, r.bottom - r.top,
						m_hWnd, NULL, GetApp()->GetInstanceHandle(), NULL);

	// Set a default font
	m_hFont = ::CreateFont(16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
	::SendMessage(m_hEdit, WM_SETFONT, (WPARAM)m_hFont, 0);
}

void CMainWindow::OnInitialUpdate()
{
	CMyDialog MyDialog(IDD_DIALOG1, m_hWnd);
	MyDialog.DoModal();
}

void CMainWindow::OnSize()
{
	CRect r = GetClientRect();

	// Resize the edit window when the main window is resized
	::MoveWindow(m_hEdit, 0, 0, r.right - r.left, r.bottom - r.top, TRUE);
}

void CMainWindow::OnAllWindowsCreated()
{
	TCHAR str[80];
	wsprintf(str, _T("%d  Windows Created"), m_nTestWindows);
	SendText(str);
	SendText(_T("Ready to run performance test"));

	int nResult = ::MessageBox(m_hWnd, _T("Start the Performance Test?"), _T("Ready"), MB_OKCANCEL | MB_ICONEXCLAMATION);
	if (nResult != IDOK) return;

	PerformanceTest();

	// Loop the performance test
	nResult = IDYES;
	while(nResult == IDYES)
	{
		nResult = ::MessageBox(m_hWnd, _T("Run Test Again?"), _T("Ready"), MB_YESNO | MB_ICONEXCLAMATION);
		if (nResult != IDYES) break;

		PerformanceTest();
	}
	SendText(_T("Testing complete"));
}

void CMainWindow::PerformanceTest()
{
	LRESULT lr = 0;
	int nMessages = 0;

	SendText(_T(""));
	TCHAR str[80];
	wsprintf(str, _T("Sending %d  Messages"), m_nTestMessages);
	SendText(str);

	// Choose a Window handle(HWND) to send the messages to
	HWND hWnd = m_pCTestWindows[(m_nTestWindows-1)/2]->GetHwnd();

	// Store the starting time
	DWORD tStart = ::GetTickCount();

	// Send the messages
	while(nMessages++ < m_nTestMessages)
        lr = ::SendMessage(hWnd, WM_TESTMESSAGE, 0, 0);

	// Calculate the time the messages took to send
	DWORD tEnd = ::GetTickCount();
	DWORD mSeconds = tEnd - tStart;

	// Display the results
	wsprintf(str, _T("%d milliseconds to process %d messages"), mSeconds, m_nTestMessages);
	SendText(str);

	wsprintf(str, _T("%d total messages sent\n"), lr);
	TRACE(str);

	MessageBox(str, _T("Info"), MB_OK);
}

void CMainWindow::SendText(LPCTSTR str)
{
	// Send text to the Edit window
	::SendMessage(m_hEdit, EM_REPLACESEL,  (WPARAM)FALSE, (LPARAM)str);
	::SendMessage(m_hEdit, EM_REPLACESEL,  (WPARAM)FALSE, (LPARAM)_T("\r\n"));
	::SendMessage(m_hEdit, EM_SCROLLCARET, (WPARAM)0,     (LPARAM)0);

	TRACE(str);
	TRACE(_T("\n"));
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
			//Close the test windows
			for (int i = 0 ; i < m_nTestWindows ; i++)
				::SendMessage(m_pCTestWindows[i]->GetHwnd(), WM_CLOSE, 0, 0);
		}
		break;

	case WM_DESTROY:
		// End the program.
		::PostQuitMessage(0);
		break;

	case WM_SIZE:
		OnSize();
		break;

	case WM_WINDOWCREATED:
		// Message recieved when a test window is created
		if (++nWindowsCreated == m_nTestWindows)
			OnAllWindowsCreated();
		break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

