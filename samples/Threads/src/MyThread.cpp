/////////////////////////////
// MyThread.cpp
//

#include "stdafx.h"
#include "MyThread.h"

//////////////////////////////////
// CMyThread function definitions.
//
CMyThread::CMyThread(int threadNumber) :  m_testWnd(threadNumber), 
                                          m_threadNumber(threadNumber)
{
}

BOOL CMyThread::InitInstance()
{
    // This function runs when the thread starts

    // Create a test window for this thread
    m_testWnd.Create();

    return TRUE;    // return TRUE to run the message loop
}
