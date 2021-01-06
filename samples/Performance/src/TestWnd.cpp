/////////////////////////////
// TestWnd.cpp
//

#include "stdafx.h"
#include "TestWnd.h"
#include "PerfApp.h"


///////////////////////////////////
// CTestWindow function definitions
//

// Constructor.
CTestWindow::CTestWindow() : m_windowCount(0)
{
}

// Creates the test window.
void CTestWindow::CreateWin(int i)
{
    m_windowCount = i + 1;
    TCHAR str[80];
    wsprintf(str, _T("Test Window %d"), m_windowCount);
    CreateEx(0, NULL, str, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        420, 50 + 4*i, 300, 200, NULL, NULL);
}

// Called after the test window is created.
void CTestWindow::OnInitialUpdate()
{
    // Get a pointer to the CMainWnd object
    CMainWindow& mainWnd = GetPerfApp()->GetMainWnd();

    // Post a message to MainWnd when the window is created.
    mainWnd.PostMessage(WM_WINDOWCREATED, 0, 0);
}

// Processes the Test Window's messages.
LRESULT CTestWindow::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    static LRESULT messages = 0;
    switch (msg)
    {
    case WM_CLOSE:
        {
            CString str;
            str.Format(str, _T("Closing test Window #%d\n"), m_windowCount);
            TRACE(str);
        }
        break;

    case WM_TESTMESSAGE:
        // return the number of WM_TESTMESSAGE messages processed so far
        return ++messages;
    }

    return WndProcDefault(msg, wparam, lparam);
}

