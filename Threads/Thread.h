//////////////////////////////////////////////////////
// Thread.h
//  Declaration of the CThread class


#ifndef THREAD_H
#define THREAD_H

#include "ThreadApp.h"
#include "TestWnd.h"


class CThread
{
public:
	CThread(int* p_iThread);
	~CThread();
	static DWORD WINAPI ThreadCallback(LPVOID pInt);
	CTestWindow* m_pTestWindow;
	HANDLE m_hThread;		// Handle of this thread

public:
	DWORD m_dwThreadID;		// ID of this thread
	int m_iThread;			// The number of this thread (1 to MAX_THREADS)

};

#endif //THREAD_H

