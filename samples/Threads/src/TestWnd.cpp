/////////////////////////////
// TestWnd.cpp
//

#include "stdafx.h"
#include "UserMessages.h"
#include "TestWnd.h"


////////////////////////////////////
// CTestWindow function definitions.
//

// Constructor.
CTestWindow::CTestWindow(int thread, HWND mainWindow)
    : m_threadNumber(thread), m_mainWindow(mainWindow)
{
}

// Create the Test Window.
HWND CTestWindow::Create(HWND parent)
{
    UNREFERENCED_PARAMETER(parent);

    CString Title;
    Title.Format( _T("Test Window %d"), m_threadNumber );
    return CreateEx(0, NULL, Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        380 + 20* m_threadNumber, 40 + 20* m_threadNumber, 300, 200, 0, NULL);
}

// Called when the test window is closed.
void CTestWindow::OnClose()
{
    // Destroy the window.
    Destroy();

    // Close the window's thread.
    ::PostMessage(m_mainWindow, UWM_CLOSETHREAD, m_threadNumber, 0);
}

// Called after the window is created.
void CTestWindow::OnInitialUpdate()
{
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Post a message to MainWnd when the window is created. The MainWnd window
    //  is in a different thread, so PostMessage is preferred over SendMessage.
    //  SendMessage would wait for the MainWnd thread to respond.
    ::PostMessage(m_mainWindow, UWM_WINDOWCREATED, 0, 0);
}

