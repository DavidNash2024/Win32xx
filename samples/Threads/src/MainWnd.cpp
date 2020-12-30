/////////////////////////////
// MainWnd.cpp
//

#include "stdafx.h"
#include "MainWnd.h"

////////////////////////////////////
// CMainWindow function definitions.
//

// Constructor.
CMainWindow::CMainWindow() : m_windowsCount(0)
{
    // Set the number of threads
    m_maxWindows = 20;

    // A couple of notes in case you're tempted to test how many threads with test windows can be created ...

    // Note 1: Each process has a limited amount of resources allocated to it. Attempts to create
    //         more than say 1000 threads in a process might fail and throw a CWinException.
    // Note 2: All our threads belong to the one process.
    // Note 3: Creating (or destroying) more than say 200 windows may temporarily stress the Explorer process.
    // Note 4: This sample is intended as "proof of concept" only. A well written program should not require 20 GUI threads!
}

// Appends the specified text to the edit control.
void CMainWindow::AppendText(LPCTSTR text)
{
    // Append Line Feed
    int length = m_edit.GetWindowTextLength();
    if (length > 0)
        m_edit.AppendText(_T("\r\n"));

    // Append text
    m_edit.AppendText(text);

    // Also send output to the debugger for display
    TRACE(text);
    TRACE(_T("\n"));
}

// Create the window.
HWND CMainWindow::Create(HWND parent)
{
    CString str = _T("Main Thread Window");

    // Create the main window
    CRect rc(20 , 50, 400, 500);
    return CreateEx(WS_EX_TOPMOST, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        rc, parent, 0);
}

// Called when the window is created.
int CMainWindow::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

    // Create the Edit child window to display text.
    m_edit.Create(*this);


    // Create each CMyThread object.
    for (int i = 1 ; i <= m_maxWindows ; i++)
    {
        CMyThread* pMyThread = new CMyThread(i);
        m_threads.push_back(pMyThread);
    }

    // Create the threads belonging to the MyThread objects.
    // Each thread creates a TestWindow when it runs.
    std::vector<MyThreadPtr>::iterator iter;
    for (iter = m_threads.begin(); iter < m_threads.end(); ++iter)
    {
        try
        {
            (*iter)->CreateThread();

            CString str;
            str.Format( _T("Thread %d started "), (*iter)->GetThreadNumber() );
            AppendText(str);
        }

        catch (CWinException &e)
        {
            // Display the exception and allow the program to continue
            CString Error = CString(e.GetText()) + "\n" + CString(e.GetErrorString());
            ::MessageBox(NULL, Error, AtoT(e.what()), MB_ICONERROR) ;
        }
    }

    return 0;
}

// Called when the window is closed.
void CMainWindow::OnClose()
{
    // Close each test window thread.
    std::vector<MyThreadPtr>::iterator iter;
    for (iter = m_threads.begin(); iter < m_threads.end(); ++iter)
    {
        (*iter)->CloseThread();
    }

    Destroy();
}

// Called when the window is destroyed.
void CMainWindow::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}

// Called when all test windows are created.
void CMainWindow::OnAllWindowsCreated()
{
    CString str;
    str.Format( _T("%d Test windows created in seperate threads"), m_maxWindows );
    AppendText(str);
}

// Called when a test window is created.
void CMainWindow::OnWindowCreated()
{
    CString str;
    ++m_windowsCount;
    str.Format( _T("Created Window %d"), m_windowsCount);
    AppendText(str);

    if (m_windowsCount == m_maxWindows)
        OnAllWindowsCreated();
}

// Process this window's messages.
LRESULT CMainWindow::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_WINDOWCREATED:
        OnWindowCreated();
        break;
    case WM_SIZE:
        m_edit.SetWindowPos(0, GetClientRect(), 0);
        break;
    }

    return WndProcDefault(msg, wparam, lparam);
}

