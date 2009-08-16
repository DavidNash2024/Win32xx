//////////////////////////////////////////////
// Thread.cpp
//  Definitions for the CThread class


#include "Thread.h"


CThread::CThread(int nValue) : m_dwThreadID(0), m_hThread(0)
{
	try
	{		
		m_nValue = nValue;

		// Create the thread.
		m_hThread = ::CreateThread(NULL, 0, CThread::ThreadCallback, (LPVOID) this, 0, &m_dwThreadID);
		if (!m_hThread)
			throw CWinException(_T("Failed to create thread"));
	}

	catch (const CWinException &e)
	{
		e.MessageBox();
	}

	catch (...)
	{
		DebugErrMsg(_T("Exception in CThread::CThread"));
		throw;	// Rethrow unknown exception
	}
}

CThread::~CThread()
{
	// close the thread's handle
	::CloseHandle(m_hThread);
}

DWORD WINAPI CThread::ThreadCallback(LPVOID pCThread)
// This function is called automatically when the thread is started
{
	// Get the pointer for this CThread object 
	CThread* pThread = (CThread*)pCThread;
	
	int i = pThread->m_nValue;
	TCHAR str[80];
	wsprintf(str, _T("Thread %d started\n"), i + 1);
	TRACE(str);

	// Create a test window for this thread
	pThread->m_TestWindow.CreateWin(i);

	// Each thread with a window has its own message loop
	// The message loop runs until the thread's window is destroyed
	return GetApp()->MessageLoop();
}
