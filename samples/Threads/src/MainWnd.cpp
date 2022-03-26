/////////////////////////////
// MainWnd.cpp
//

#include "stdafx.h"
#include "UserMessages.h"
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
    //         more than say 500 threads in a process might fail and throw a CWinException.
    // Note 2: All our threads belong to the one process.
    // Note 3: Creating (or destroying) more than say 200 windows may temporarily stress the Explorer process.
    // Note 4: This sample is intended as "proof of concept" only. A well written program should not require 20 GUI threads!
}

// Appends the specified text to the edit control.
void CMainWindow::AppendText(LPCTSTR text)
{
    // Append Line Feed.
    if (m_edit.IsWindow())
    {
        int length = m_edit.GetWindowTextLength();
        if (length)
            m_edit.AppendText(_T("\r\n"));

        // Append text.
        m_edit.AppendText(text);
    }
}

// Create the main window.
HWND CMainWindow::Create(HWND parent)
{
    CString str = _T("Main Thread Window");

    // Create the main window.
    CRect rc(20 , 50, 400, 500);
    return CreateEx(WS_EX_TOPMOST, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        rc, parent, 0);
}

// Called when the window is created.
int CMainWindow::OnCreate(CREATESTRUCT&)
{
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Create the Edit child window to display text.
    m_edit.Create(*this);

    // Ensure thread safe access to the m_threads vector.
    CThreadLock lock(m_cs);

    // Create each CMyWinThread object.
    // m_threads is a vector of smart pointers to CMyWinThread.
    // CMyWinThread's destructor is called when the smart pointer
    //  goes out of scope.
    for (int i = 1 ; i <= m_maxWindows ; i++)
    {
        MyThreadPtr threadPtr(new CMyWinThread(i, GetHwnd()));
        m_threads.push_back(threadPtr);
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
            // Display the exception and allow the program to continue.
            CString Error = CString(e.GetText()) + "\n" + CString(e.GetErrorString());
            ::MessageBox(0, Error, AtoT(e.what()), MB_ICONERROR) ;
        }
    }

    return 0;
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
    str.Format( _T("%d Test windows created in separate threads"), m_maxWindows );
    AppendText(str);
}

// Called in response to a UWM_APPENDTEXT message.
LRESULT CMainWindow::OnAppendText(WPARAM wparam)
{
    CString* pStr = reinterpret_cast<CString*>(wparam);
    AppendText(pStr->c_str());
    return 0;
}

// Called in response to a UWM_CLOSETHREAD message.
LRESULT CMainWindow::OnCloseThread(WPARAM wparam)
{
    // Ensure thread safe access to the m_threads vector.
    CThreadLock lock(m_cs);

    int threadNumber = static_cast<int>(wparam);
    std::vector<MyThreadPtr>::iterator iter;
    for (iter = m_threads.begin(); iter < m_threads.end(); ++iter)
    {
        if (threadNumber == (*iter)->GetThreadNumber())
        {
            m_threads.erase(iter);
            break;
        }
    }

    return 0;
}

// Called in the main window is resized.
LRESULT CMainWindow::OnSize()
{
    m_edit.SetWindowPos(0, GetClientRect(), 0);
    return 0;
}

// Called when a test window is created.
LRESULT CMainWindow::OnWindowCreated()
{
    CString str;
    ++m_windowsCount;
    str.Format( _T("Created Window %d"), m_windowsCount);
    AppendText(str);

    if (m_windowsCount == m_maxWindows)
        OnAllWindowsCreated();

    return 0;
}

// Process this window's messages.
LRESULT CMainWindow::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_APPENDTEXT:     return OnAppendText(wparam);
        case UWM_CLOSETHREAD:    return OnCloseThread(wparam);
        case UWM_WINDOWCREATED:  return OnWindowCreated();
        case WM_SIZE:            return OnSize();
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

