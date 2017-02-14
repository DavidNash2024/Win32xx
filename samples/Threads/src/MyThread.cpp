///////////////////////////////////////
// MyThread.cpp
//  Definitions for the CMyThread class

#include "stdafx.h"
#include "MyThread.h"


CMyThread::CMyThread(int nThread) :  m_TestWnd(nThread), m_nThread(nThread)
{
}

BOOL CMyThread::InitInstance()
{
	// This function runs when the thread starts

	// Create a test window for this thread
	m_TestWnd.Create();

	return TRUE;	// return TRUE to run the message loop
}
