//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CMainWindow class

#include "stdafx.h"
#include "MainWnd.h"
#include "MyDialog.h"
#include "PerfApp.h"
#include "resource.h"


CMainWindow::CMainWindow() : m_nTestMessages(0), m_nTestWindows(0), m_nWindowsCreated(0)
{
}

CMainWindow::~CMainWindow()
{
}

HWND CMainWindow::Create(HWND hParent /*= 0*/)
{
    CString str = _T("Main Window");

    // Create the main window
    CRect rc(20, 50, 400, 300);

    return CreateEx(WS_EX_TOPMOST, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        rc, hParent, 0);
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

int CMainWindow::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

    m_Edit.Create(*this);

    return 0;
}

void CMainWindow::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}


void CMainWindow::OnInitialUpdate()
{
    CMyDialog MyDialog(IDD_DIALOG1);
    MyDialog.DoModal(*this);
}

LRESULT CMainWindow::OnSize()
{
    CRect r = GetClientRect();

    // Resize the edit window when the main window is resized
    m_Edit.MoveWindow(0, 0, r.right - r.left, r.bottom - r.top, TRUE);

    return 0L;
}


LRESULT CMainWindow::OnWindowCreated()
{
    // Message recieved when a test window is created
    if (++m_nWindowsCreated == m_nTestWindows)
        OnAllWindowsCreated();
    
    return 0L;
}

void CMainWindow::OnAllWindowsCreated()
{
    CString str;
    str.Format(_T("%d  Windows Created"), m_nTestWindows);
    SendText(str);
    SendText(_T("Ready to run performance test"));

    int nResult = MessageBox(_T("Start the Performance Test?"), _T("Ready"), MB_OKCANCEL | MB_ICONEXCLAMATION);
    if (nResult != IDOK) return;

    PerformanceTest();

    // Loop the performance test
    nResult = IDYES;
    while(nResult == IDYES)
    {
        nResult = MessageBox(_T("Run Test Again?"), _T("Ready"), MB_YESNO | MB_ICONEXCLAMATION);
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
    CString str;
    str.Format(str, _T("Sending %d  Messages"), m_nTestMessages);
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
    str.Format(_T("%d milliseconds to process %d messages"), mSeconds, m_nTestMessages);
    SendText(str);

    str.Format(_T("%d total messages sent\n"), lr);
    TRACE(str);

    MessageBox(str, _T("Info"), MB_OK);
}

void CMainWindow::SendText(LPCTSTR str)
{
    // Send text to the Edit window
    m_Edit.SendMessage(EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(str));
    m_Edit.SendMessage(EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(_T("\r\n")));
    m_Edit.SendMessage(EM_SCROLLCARET, 0, 0);

    TRACE(str);
    TRACE("\n");
}

LRESULT CMainWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:           return OnSize();
    case WM_WINDOWCREATED:  return OnWindowCreated();
    }

    return WndProcDefault(uMsg, wParam, lParam);
}

