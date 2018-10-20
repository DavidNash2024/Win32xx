//////////////////////////////////////////////
// MainWnd.cpp
//  Definitions for the CMainWindow class

#include "stdafx.h"
#include "MainWnd.h"


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

void CMainWindow::AppendText(LPCTSTR text)
{
    // This function appends text to an edit control

    // Append Line Feed
    LRESULT ndx = m_editWnd.SendMessage(WM_GETTEXTLENGTH, 0, 0);
    if (ndx)
    {
        m_editWnd.SendMessage(EM_SETSEL, ndx, ndx);
        m_editWnd.SendMessage(EM_REPLACESEL, 0, (LPARAM) (_T("\r\n")));
    }


    // Append text
    ndx = m_editWnd.SendMessage(WM_GETTEXTLENGTH, 0, 0);
    m_editWnd.SendMessage(EM_SETSEL, ndx, ndx);
    m_editWnd.SendMessage(EM_REPLACESEL, 0, (LPARAM)text);

    // Also send output to the debugger for display
    TRACE(text);
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

int CMainWindow::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

    // Create the Edit child window to display text
    m_editWnd.Create(*this);


    // Create each CMyThread object
    for (int i = 1 ; i <= m_maxWindows ; i++)
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
            ::MessageBox(NULL, Error, AtoT(e.what()), MB_ICONERROR) ;

            // return -1 here to close the main window and end the application
        //  return -1;
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
		SendMessage( (*iter)->GetTestWnd()->GetHwnd(), WM_CLOSE, 0, 0 );
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
    str.Format( _T("%d Test windows created in seperate threads"), m_maxWindows );
    AppendText(str);
}

void CMainWindow::OnWindowCreated()
{
    // Message recieved when a test window is created
    CString str;
    ++m_windowsCount;
    str.Format( _T("Created Window %d"), m_windowsCount);
    AppendText(str);

    if (m_windowsCount == m_maxWindows)
        OnAllWindowsCreated();
}

LRESULT CMainWindow::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_WINDOWCREATED:
        OnWindowCreated();
        break;
    case WM_SIZE:
        m_editWnd.SetWindowPos(0, GetClientRect(), 0);
        break;
    }

    return WndProcDefault(msg, wparam, lparam);
}

