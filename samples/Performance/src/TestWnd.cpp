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
    CString str;
    str << "Test Window " << m_windowCount;

    // Create a test window without a close button.
    CRect rc(420, 50 + 4 * i, 300, 200);
    CRect rcDPI = DPIScaleRect(rc);
    CreateEx(0, NULL, str, WS_VISIBLE, rcDPI, 0, NULL);
}

// Called when the window is created.
int CTestWindow::OnCreate(CREATESTRUCT&)
{
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    return 0;
}

// Called after the test window is created.
void CTestWindow::OnInitialUpdate()
{
    // Get a pointer to the CMainWnd object
    CMainWindow& mainWnd = GetPerfApp()->GetMainWnd();

    // Post a message to MainWnd when the window is created.
    mainWnd.PostMessage(WM_WINDOWCREATED, 0, 0);
}

LRESULT CTestWindow::OnTestMessage()
{
    static LRESULT messages = 0;

    // Return the number of WM_TESTMESSAGE messages processed so far.
    return ++messages;
}

// Processes the Test Window's messages.
LRESULT CTestWindow::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_TESTMESSAGE:   return OnTestMessage();
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

