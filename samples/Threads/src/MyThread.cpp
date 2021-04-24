/////////////////////////////
// MyThread.cpp
//

#include "stdafx.h"
#include "UserMessages.h"
#include "MyThread.h"


//////////////////////////////////
// CMyThread function definitions.
//
CMyThread::CMyThread(int threadNumber, HWND mainWindow)
    :  m_testWnd(threadNumber, mainWindow),
       m_threadNumber(threadNumber),
       m_mainWindow(mainWindow)
{
}

CMyThread::~CMyThread()
{
    CloseThread();
    CString str;
    str.Format(_T("Thread %d has ended\n"), m_threadNumber);
    Trace(str);
}

// Close this thread.
void CMyThread::CloseThread()
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
BOOL CMyThread::InitInstance()
{
    // Create a test window for this thread.
    m_testWnd.Create();

    return TRUE;    // return TRUE to run the message loop.
}
