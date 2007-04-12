//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CMainWindow class


#include "PerfApp.h"
#include "MainWnd.h"
#include "resource.h"


CMainWindow::CMainWindow() : m_hEdit(NULL), m_hFont(NULL), m_nTestMessages(0), m_pCTestWindows(NULL)
{
}

CMainWindow::~CMainWindow()
{
	if (m_hFont)
		::DeleteObject(m_hFont);
	
	for (int i = 0 ; i < m_nTestWindows; i++)
	{
		delete m_pCTestWindows[i];
	}
	delete []m_pCTestWindows;
}

void CMainWindow::Create()
{
	TCHAR str[80];
	wsprintf(str, TEXT("Main Window"));
	
	// Create the main window
	CreateEx(WS_EX_TOPMOST, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		20 , 50, 400, 300, NULL, NULL);
}

void CMainWindow::CreateTestWindows(int nWindows)
{
	m_nTestWindows = nWindows;
	m_pCTestWindows = new CTestWindow*[nWindows];

	for (int i = 0 ; i < nWindows ; i++)
	{
		// Create the test windows 
		m_pCTestWindows[i] = new CTestWindow();
		m_pCTestWindows[i]->CreateWin(i);
	}
}

void CMainWindow::OnCreate()
{
	RECT r;
	::GetClientRect(m_hWnd, &r);
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

	// Create an Edit window over the client area of the main window
	m_hEdit = ::CreateWindowEx(0L, TEXT("Edit"), TEXT(""), dwStyle, r.left, r.top, r.right - r.left, r.bottom - r.top,
						m_hWnd, NULL, GetApp()->GetInstanceHandle(), NULL);

	// Set a default font
	m_hFont = ::CreateFont(16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, TEXT("Courier New"));
	::SendMessage(m_hEdit, WM_SETFONT, (WPARAM)m_hFont, 0);
}

void CMainWindow::OnInitialUpdate()
{
	CMyDialog MyDialog(IDD_DIALOG1, m_hWnd);
	MyDialog.DoModal();
}

void CMainWindow::OnSize()
{
	RECT r;
	::GetClientRect(m_hWnd, &r);
	
	// Resize the edit window when the main window is resized
	::MoveWindow(m_hEdit, 0, 0, r.right - r.left, r.bottom - r.top, TRUE);
}

void CMainWindow::OnAllWindowsCreated()
{
	TCHAR str[80];
	wsprintf(str, TEXT("%d  Windows Created"), m_nTestWindows);	
	SendText(str);
	SendText(TEXT("Ready to run performance test"));

	int nResult = ::MessageBox(m_hWnd, TEXT("Start the Performance Test?"), TEXT("Ready"), MB_OKCANCEL | MB_ICONEXCLAMATION);	
	if (nResult != IDOK) return;

	PerformanceTest();
	
	// Loop the performance test
	nResult = IDYES;
	while(nResult == IDYES)
	{
		nResult = ::MessageBox(m_hWnd, TEXT("Run Test Again?"), TEXT("Ready"), MB_YESNO | MB_ICONEXCLAMATION);
		if (nResult != IDYES) break;
			
		PerformanceTest();
	}
	SendText(TEXT("Testing complete")); 
}

void CMainWindow::PerformanceTest()
{
	TCHAR str[80];
	LRESULT lr = 0;
	int nMessages = 0;

	SendText(TEXT(""));
	wsprintf(str, TEXT("Sending %d Messages"), m_nTestMessages);
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
	wsprintf(str, TEXT("%d  milliseconds to process %d messages"), mSeconds, m_nTestMessages);
	SendText(str);

	MessageBox(m_hWnd, str, TEXT("Info"), MB_OK);
	wsprintf(str, TEXT("%d total messages sent "), lr);
	TRACE(str); 
}

void CMainWindow::SendText(LPCTSTR str)
{
	// Send text to the Edit window
	::SendMessage(m_hEdit, EM_REPLACESEL,  (WPARAM)FALSE, (LPARAM)str);
	::SendMessage(m_hEdit, EM_REPLACESEL,  (WPARAM)FALSE, (LPARAM)TEXT("\r\n"));
	::SendMessage(m_hEdit, EM_SCROLLCARET, (WPARAM)0,     (LPARAM)0);

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
			//Close the test windows
			for (int i = 0 ; i < m_nTestWindows ; i++)
				::SendMessage(m_pCTestWindows[i]->GetHwnd(), WM_CLOSE, 0, 0);
		}
		break;
	
	case WM_DESTROY:	
		// Post the WM_QUIT message to terminate the program.
		::PostQuitMessage(0);
		return 0L;
	
	case WM_SIZE:
		OnSize();
		return 0;
	
	case WM_WINDOWCREATED:
		// Message recieved when a test window is created
		if (++nWindowsCreated == m_nTestWindows)
			OnAllWindowsCreated();
		return 0L;
	}

	//Use the CWnd default message handling for remaining messages
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}

