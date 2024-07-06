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
HWND CTestWindow::Create(HWND)
{
    CString Title;
    Title.Format( _T("Test Window %d"), m_threadNumber );
    int left = 380 + 20 * m_threadNumber;
    int top = 40 + 20 * m_threadNumber;
    CRect rc(left, top, left + 300, top + 200);
    rc = DpiScaleRect(rc);
    return CreateEx(0, NULL, Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, rc, 0, 0);
}

// Called when the test window is manually closed by the user.
void CTestWindow::OnClose()
{
    // Destroy the window.
    Destroy();

    // Ask the main window to close this window's thread.
    // Must post this message.
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

// Handle the window's messages.
LRESULT CTestWindow::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}
