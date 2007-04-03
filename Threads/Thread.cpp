//////////////////////////////////////////////
// Thread.cpp
//  Definitions for the CThread class

#include "Thread.h"


CThread::CThread(int* p_iThread)
{
	try
	{
		m_dwThreadID = 0;
		m_pTestWindow = NULL;
		m_iThread = *p_iThread;
		
		// Create the thread.
		m_hThread = ::CreateThread(NULL, 0, CThread::ThreadCallback, (LPVOID) this, 0, &m_dwThreadID);
		if (!m_hThread)
			throw CWinException(TEXT("Failed to create thread"));
	}

	catch (const CWinException &e)
	{
		e.MessageBox();
	}

	catch (...)
	{
		DebugErrMsg(TEXT("Exception in CThread::CThread"));
		throw;	// Rethrow unknown exception
	}
}

CThread::~CThread()
{
	// delete the Test Window CWnd object
	delete m_pTestWindow;
	
	// close the thread's handle
	::CloseHandle(m_hThread);
}

DWORD WINAPI CThread::ThreadCallback(LPVOID pThread)
// This function is called automatically when the thread is created
{
	// Get the pointer for this CThread object 
	CThread* pCThread = (CThread*)pThread;
	
	int i = pCThread->m_iThread;
	TCHAR str[80];
	wsprintf(str, TEXT("Thread #%d started"), i + 1);
	TRACE(str);

	// Create a test window for this thread
	pCThread->m_pTestWindow = new CTestWindow;
	pCThread->m_pTestWindow->CreateWin(i);

	// Each thread with a window has its own message loop
	// The message loop runs until the thread's window is destroyed
	return GetApp()->MessageLoop();
}
