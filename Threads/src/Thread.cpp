//////////////////////////////////////////////
// Thread.cpp
//  Definitions for the CThread class


#include "stdafx.h"
#include <process.h>
#include "Thread.h"


CThread::CThread(int nValue) : m_ThreadID(0), m_hThread(0)
{
	m_nValue = nValue;

	// Create the thread. It is created in a suspended stated.
	// Note: _beginthreadex will be undefined if a single-threaded run-time library is used. Use a Multithreaded run-time. 
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadCallback, (LPVOID) this, CREATE_SUSPENDED, &m_ThreadID);
	if (!m_hThread)
		throw CWinException(_T("Failed to create thread in CThread::CThread"));
}

CThread::~CThread()
{
	// Post a quite message to end the thread
	Stop();
	
	// Close the thread's handle
	::CloseHandle(m_hThread);
}

void CThread::Start()
{
	// This starts the thread running. It was suspended when created.
	ResumeThread(m_hThread);
}

void CThread::Stop()
{
	// Posts a quit message to the thread to end it.
	if (m_ThreadID)
		::PostThreadMessage(m_ThreadID, WM_QUIT,0,0);
}

UINT WINAPI CThread::ThreadCallback(LPVOID pCThread)
// This function is the function that the thread runs 
{
	try
	{
		// Get the pointer for this CThread object 
		CThread* pThread = (CThread*)pCThread;
		
		int i = pThread->m_nValue;

		// Create a test window for this thread
		pThread->m_TestWindow.CreateWin(i);

		// Each thread with a window has its own message loop
		// The message loop runs until the thread's window is destroyed
		return GetApp()->MessageLoop();
	}

	catch (...)
	{
		DebugErrMsg(_T("Exception in CThread::ThreadCallback"));
		throw;
	}
}
