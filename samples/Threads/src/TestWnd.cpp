/////////////////////////////
// TestWnd.cpp
//

#include "stdafx.h"
#include "TestWnd.h"
#include "ThreadApp.h"

////////////////////////////////////
// CTestWindow function definitions.
//

// Constructor.
CTestWindow::CTestWindow(int window) : m_window(window)
{
}

// Create the Test Window.
HWND CTestWindow::Create(HWND parent)
{
    UNREFERENCED_PARAMETER(parent);

    CString Title;
    Title.Format( _T("Test Window %d"), m_window );
    return CreateEx(0, NULL, Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        380 + 20*m_window, 40 + 20*m_window, 300, 200, NULL, NULL);
}

// Called when the test window is closed.
void CTestWindow::OnClose()
{
    CMainWindow& MainWnd = GetThreadApp()->GetMainWnd();
    CString str;
    str.Format( _T("Closing test Window %d"), m_window );
    MainWnd.AppendText(str);

    Destroy();
}

// Called after the window is created.
void CTestWindow::OnInitialUpdate()
{
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Get a pointer to the CMainWindow object
    CMainWindow& MainWnd = GetThreadApp()->GetMainWnd();

    // Post a message to MainWnd when the window is created. The MainWnd window
    //  is in a different thread, so PostMessage is preferred over SendMessage.
    //  SendMessage would wait for the MainWnd thread to respond.
    MainWnd.PostMessage(WM_WINDOWCREATED, 0, 0);
}

