//////////////////////////////////////////////
// Thread.cpp
//  Definitions for the CThread class

#include <process.h>
#include "Thread.h"


CThread::CThread(int nValue) : m_ThreadID(0), m_hThread(0)
{
	try
	{		
		m_nValue = nValue;

		// Create the thread.
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadCallback, (LPVOID) this, CREATE_SUSPENDED, &m_ThreadID);
		if (!m_hThread)
			throw CWinException(_T("Failed to create thread in CThread::CThread"));
	}

	catch (const CWinException &e)
	{
		::PostThreadMessage(m_ThreadID, WM_QUIT,0,0);
		::CloseHandle(m_hThread);
		
		e.MessageBox();
		throw;
	}

	catch (...)
	{
		::PostThreadMessage(m_ThreadID, WM_QUIT,0,0);
		::CloseHandle(m_hThread);

		DebugErrMsg(_T("Exception in CThread::CThread"));
		throw;	// Rethrow unknown exception
	}
}

CThread::~CThread()
{
	// Post a quite message to end the thread
	::PostThreadMessage(m_ThreadID, WM_QUIT, 0, 0);
	
	// Close the thread's handle
	::CloseHandle(m_hThread);
}

UINT WINAPI CThread::ThreadCallback(LPVOID pCThread)
// This function is called automatically when the thread is started
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
