/////////////////////////////
// MyThread.cpp
//

#include "stdafx.h"
#include "MyThread.h"

#include "ThreadApp.h"

//////////////////////////////////
// CMyThread function definitions.
//
CMyThread::CMyThread(int threadNumber) :  m_testWnd(threadNumber),
                                          m_threadNumber(threadNumber)
{
}

// Close this thread.
void CMyThread::CloseThread()
{
    CMainWindow& MainWnd = GetThreadApp()->GetMainWnd();
    CString str;
    if (m_testWnd.IsWindow())
    {
        m_testWnd.Destroy();
        str.Format(_T("Destroyed Window %d"), m_threadNumber);
        MainWnd.AppendText(str);
    }

    // All the thread's windows have ended, so we can post a WM_QUIT to end the thread. 
    PostThreadMessage(WM_QUIT, 0, 0);
    WaitForSingleObject(*this, INFINITE);
    str.Format(_T("Thread %d has ended"), m_threadNumber);
    MainWnd.AppendText(str);
}

// Called when the thread starts.
BOOL CMyThread::InitInstance()
{
    // Create a test window for this thread.
    m_testWnd.Create();

    return TRUE;    // return TRUE to run the message loop.
}
