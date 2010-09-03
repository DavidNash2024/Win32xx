//////////////////////////////////////////////////////
// Thread.h
//  Declaration of the CMyThread class

#ifndef THREAD_H
#define THREAD_H

#include "TestWnd.h"


class CMyThread : public CWinThread
{
public:
	CMyThread(int nValue);
	virtual ~CMyThread() {};
	virtual BOOL InitInstance();
	virtual CTestWindow& GetTestWindow() { return m_TestWindow; }

private:
	int m_nValue;				// a value associated with this thread
	CTestWindow m_TestWindow;	// The CWnd object for this thread
};

#endif //THREAD_H

