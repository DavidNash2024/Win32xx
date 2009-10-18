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
	virtual HANDLE GetHandle() { return m_hThread; }
	virtual UINT GetThreadID() { return m_ThreadID; }
	virtual CTestWindow& GetTestWindow() {return m_TestWindow;}
	virtual void Start();
	virtual void Stop();
	static UINT WINAPI ThreadCallback(LPVOID pCThread);

private:
	UINT m_ThreadID;			// ID of this thread
	int m_nValue;				// a value associated with this thread
	HANDLE m_hThread;			// Handle of this thread
	CTestWindow m_TestWindow;	// The CWnd object for this thread
};

#endif //THREAD_H

