//////////////////////////////////////////////
// Thread.cpp
//  Definitions for the CMyThread class

#include "stdafx.h"
#include <process.h>
#include "Thread.h"


CMyThread::CMyThread(int nValue) : m_nValue(nValue)
{
}

BOOL CMyThread::InitInstance()
{
	// This function runs when the thread starts

	// Create a test window for this thread
	m_TestWindow.CreateWin(m_nValue);

	return TRUE;	// return TRUE to run the message loop
}

