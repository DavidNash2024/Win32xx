/////////////////////////////
// MyThread.cpp
//

#include "stdafx.h"
#include "UserMessages.h"
#include "MyThread.h"


//////////////////////////////////
// CMyWinThread function definitions.
//
CMyWinThread::CMyWinThread(int threadNumber, HWND mainWindow)
    :  m_testWnd(threadNumber, mainWindow),
       m_threadNumber(threadNumber),
       m_mainWindow(mainWindow)
{
}

CMyWinThread::~CMyWinThread()
{
    CloseThread();
    CString str;
    str.Format(_T("Thread %d has ended\n"), m_threadNumber);
    Trace(str);
}

// Close this thread.
void CMyWinThread::CloseThread()
{
    if (m_testWnd.IsWindow())
        m_testWnd.Close();

    CString str;
    str.Format(_T("Test Window %d destroyed"), m_threadNumber);

    // We use SendMessage here to keep the str pointer in scope.
    ::SendMessage(m_mainWindow, UWM_APPENDTEXT, (WPARAM)&str, 0);

    // Also send the text to the debugger output.
    Trace(str);
    Trace("\n");

    // All the thread's windows have ended, so we can post a WM_QUIT to end the thread.
    PostThreadMessage(WM_QUIT, 0, 0);

    // Wait for the thread to end.
    ::WaitForSingleObject(*this, INFINITE);
}

// Called when the thread starts.
BOOL CMyWinThread::InitInstance()
{
    try
    {
        // Create a test window for this thread.
        m_testWnd.Create();
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and quit.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return FALSE;
    }

    return TRUE;    // return TRUE to run the message loop.
}
