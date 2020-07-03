//////////////////////////////////////////////
// TestWnd.cpp
//  Definitions for the CTestWindow class

#include "stdafx.h"
#include "TestWnd.h"
#include "ThreadApp.h"


CTestWindow::CTestWindow(int nWindow) : m_nWindow(nWindow)
{
}

HWND CTestWindow::Create(HWND hParent)
{
    UNREFERENCED_PARAMETER(hParent);

    CString Title;
    Title.Format( _T("Test Window %d"), m_nWindow );
    return CreateEx(0, NULL, Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        380 + 20*m_nWindow, 40 + 20*m_nWindow, 300, 200, NULL, NULL);
}

void CTestWindow::OnClose()
{
    CMainWindow& MainWnd = GetThreadApp()->GetMainWnd();
    CString str;
    str.Format( _T("Closing test Window %d"), m_nWindow );
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


