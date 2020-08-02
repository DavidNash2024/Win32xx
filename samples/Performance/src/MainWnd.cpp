//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CMainWindow class

#include "stdafx.h"
#include "MainWnd.h"
#include "MyDialog.h"
#include "PerfApp.h"
#include "resource.h"


CMainWindow::CMainWindow() : m_testMessages(0), m_testWindows(0), m_windowsCreated(0)
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

void CMainWindow::CreateTestWindows(int windows)
{
    m_testWindows = windows;

    for (int i = 0 ; i < windows; i++)
    {
        // Create the test windows
        TestWindowPtr pTestWindow = new CTestWindow();
        m_pTestWindows.push_back(pTestWindow);
        m_pTestWindows[i]->CreateWin(i);
    }
}

int CMainWindow::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

    m_edit.Create(*this);

    return 0;
}

void CMainWindow::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}


void CMainWindow::OnInitialUpdate()
{
    CMyDialog myDialog(IDD_DIALOG1);
    myDialog.DoModal(*this);
}

LRESULT CMainWindow::OnSize()
{
    CRect r = GetClientRect();

    // Resize the edit window when the main window is resized
    m_edit.MoveWindow(0, 0, r.right - r.left, r.bottom - r.top, TRUE);

    return 0;
}


LRESULT CMainWindow::OnWindowCreated()
{
    // Message recieved when a test window is created
    if (++m_windowsCreated == m_testWindows)
        OnAllWindowsCreated();
    
    return 0;
}

void CMainWindow::OnAllWindowsCreated()
{
    CString str;
    str.Format(_T("%d  Windows Created"), m_testWindows);
    SendText(str);
    SendText(_T("Ready to run performance test"));

    int result = MessageBox(_T("Start the Performance Test?"), _T("Ready"), MB_OKCANCEL | MB_ICONEXCLAMATION);
    if (result != IDOK) return;

    PerformanceTest();

    // Loop the performance test
    result = IDYES;
    while(result == IDYES)
    {
        result = MessageBox(_T("Run Test Again?"), _T("Ready"), MB_YESNO | MB_ICONEXCLAMATION);
        if (result != IDYES) break;

        PerformanceTest();
    }
    SendText(_T("Testing complete"));
}

void CMainWindow::PerformanceTest()
{
    LRESULT result = 0;
    int messages = 0;

    CString str;
    str.Format(str, _T("Sending %d  Messages\n"), m_testMessages);
    SendText(str);

    // Choose a Window handle(HWND) to send the messages to
    HWND hWnd = m_pTestWindows[(m_testWindows-1)/2]->GetHwnd();

    // Store the starting time
    DWORD tStart = ::GetTickCount();

    // Send the messages
    while(messages++ < m_testMessages)
        result = ::SendMessage(hWnd, WM_TESTMESSAGE, 0, 0);

    // Calculate the time the messages took to send
    DWORD tEnd = ::GetTickCount();
    DWORD mSeconds = tEnd - tStart;

    // Display the results
    str.Format(_T("%d milliseconds to process %d messages"), mSeconds, m_testMessages);
    SendText(str);

    str.Format(_T("%d total messages sent\n"), result);
    TRACE(str);

    MessageBox(str, _T("Info"), MB_OK);
}

void CMainWindow::SendText(LPCTSTR str)
{
    // Send text to the Edit window
    m_edit.AppendText(str);
    m_edit.AppendText(_T("\r\n"));

    TRACE(str);
    TRACE("\n");
}

LRESULT CMainWindow::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_SIZE:           return OnSize();
    case WM_WINDOWCREATED:  return OnWindowCreated();
    }

    return WndProcDefault(msg, wparam, lparam);
}

