//////////////////////////////////////////////
// Thread.cpp
//  Definitions for the CMyThread class

#include "stdafx.h"
#include <process.h>
#include "Thread.h"


CMyThread::CMyThread(int nValue)
{
	m_nValue = nValue;
}

void CMyThread::PreCreateThread(DWORD &dwCreateFlags, UINT &nStackSize, LPSECURITY_ATTRIBUTES &pSecurityAttrs)
{
	// This function will be called automatically by CWinThread, and overriding it is optional.
	// It provides an opportunity to set various thread parameters prior to creating the thread.
	// You are not required to set these parameters, any paramters which aren't specified are
	// set to the following defaults.
	dwCreateFlags = CREATE_SUSPENDED;	// CREATE_SUSPENDED or 0
	nStackSize = 0;						// Stack size for a new thread or 0
	pSecurityAttrs = 0;					// pointer to SECURITY_DESCRIPTOR structure
}

BOOL CMyThread::InitInstance()
{
	int i = m_nValue;

	// Create a test window for this thread
	m_TestWindow.CreateWin(i);

	return TRUE;	// return TRUE to run the message loop
}

