/////////////////////////////
// TestWnd.cpp
//

#include "stdafx.h"
#include "TestWnd.h"
#include "ThreadApp.h"

////////////////////////////////////
// CTestWindow function definitions.
//
CTestWindow::CTestWindow(int window) : m_window(window)
{
}

HWND CTestWindow::Create(HWND parent)
{
    UNREFERENCED_PARAMETER(parent);

    CString Title;
    Title.Format( _T("Test Window %d"), m_window );
    return CreateEx(0, NULL, Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        380 + 20*m_window, 40 + 20*m_window, 300, 200, NULL, NULL);
}

void CTestWindow::OnClose()
{
    CMainWindow& MainWnd = GetThreadApp()->GetMainWnd();
    CString str;
    str.Format( _T("Closing test Window %d"), m_window );
    MainWnd.AppendText(str);

    CWnd::OnClose();
}
void CTestWindow::OnDestroy()
{
    CMainWindow& MainWnd = GetThreadApp()->GetMainWnd();
    CString str(_T("Terminating the thread"));
    MainWnd.AppendText(str);

    // Terminate the thread.
    ::PostQuitMessage(0);
}

void CTestWindow::OnInitialUpdate()
{
    // Get a pointer to the CMainWindow object
    CMainWindow& MainWnd = GetThreadApp()->GetMainWnd();

    // Post a message to MainWnd when the window is created. The MainWnd window
    //  is in a different thread, so PostMessage is preferred over SendMessage.
    //  SendMessage would wait for the MainWnd thread to respond.
    MainWnd.PostMessage(WM_WINDOWCREATED, 0, 0);
}


