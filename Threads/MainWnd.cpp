//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CThreadWnd class


#include "ThreadApp.h"
#include "MainWnd.h"
#include "Thread.h"
#include "resource.h"


CMainWindow::CMainWindow() : m_hEdit(NULL), m_hFont(NULL), m_pCThreads(NULL), m_nThreads(0), m_nTestMessages(0)
{
}

CMainWindow::~CMainWindow()
{
	for (int i = 0 ; i < m_nThreads; i++)
		delete m_pCThreads[i];

	delete []m_pCThreads;

	if (m_hFont)
		::DeleteObject(m_hFont);
}

void CMainWindow::Create()
{
	TCHAR str[80];
	wsprintf(str, TEXT("Main Thread Window"));
	
	// Create the main window
	CreateEx(WS_EX_TOPMOST, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		20 , 50, 400, 300, NULL, NULL);
}

void CMainWindow::CreateThreads(int nThreads)
{
	m_nThreads = nThreads;
	m_pCThreads = new CThread*[nThreads];

	for (int i = 0 ; i < nThreads ; i++)
	{
		// Create the thread and store the CThread pointer
		m_pCThreads[i] = new CThread(i);
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
	wsprintf(str, TEXT("%d  Windows Created"), m_nThreads);	
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
	HWND hWnd = m_pCThreads[(m_nThreads-1)/2]->GetTestWindow()->GetHwnd();
	
	// Store the starting time
	DWORD tStart = ::GetTickCount();

	// Send the messages
	while(nMessages++ < m_nTestMessages)
        lr = ::SendMessage(hWnd, WM_TESTMESSAGE, 0, 0);

	// Calculate the time the messages took to send
	DWORD tEnd = ::GetTickCount();
	DWORD mSeconds = tEnd - tStart;

	// Display the results
	wsprintf(str, "%d  milliseconds to process %d messages", mSeconds, m_nTestMessages);
	SendText(str);

	MessageBox(m_hWnd, str, TEXT("Info"), MB_OK);
	wsprintf(str, "%d total messages sent ", lr);
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
			//Close the thread windows
			for (int i = 0 ; i < m_nThreads ; i++)
				::SendMessage(m_pCThreads[i]->GetTestWindow()->GetHwnd(), WM_CLOSE, 0, 0);
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
		// Message recieved when a test window is created
		if (++nWindowsCreated == m_nThreads)
			OnAllWindowsCreated();
		return 0L;
	}

	//Use the CWnd default message handling for remaining messages
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}

