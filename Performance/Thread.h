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
	CThread(int nValue);
	virtual ~CThread();
	static DWORD WINAPI ThreadCallback(LPVOID pCThread);
	virtual CTestWindow* GetTestWindow() {return m_pTestWindow;}

private:
	DWORD m_dwThreadID;			// ID of this thread
	int m_nValue;				// a value associated with this thread
	HANDLE m_hThread;			// Handle of this thread
	CTestWindow* m_pTestWindow;	// A pointer to the CWnd object created by this thread
};

#endif //THREAD_H

