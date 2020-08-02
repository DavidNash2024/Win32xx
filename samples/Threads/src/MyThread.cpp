///////////////////////////////////////
// MyThread.cpp
//  Definitions for the CMyThread class

#include "stdafx.h"
#include "MyThread.h"


CMyThread::CMyThread(int thread) :  m_testWnd(thread), m_thread(thread)
{
}

BOOL CMyThread::InitInstance()
{
    // This function runs when the thread starts

    // Create a test window for this thread
    m_testWnd.Create();

    return TRUE;    // return TRUE to run the message loop
}
