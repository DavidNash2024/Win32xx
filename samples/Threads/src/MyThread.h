/////////////////////////////
// MyThread.h
//

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "TestWnd.h"


/////////////////////////////////////////////////////////
// CMyThread manages a windows thread. It runs a separate
// message loop and creates a test window when it starts.
class CMyThread : public CWinThread
{
public:
    CMyThread(int thread);
    virtual ~CMyThread() {}

    virtual BOOL InitInstance();
    CTestWindow* GetTestWnd()   { return &m_testWnd; }
    int GetThreadCount()        { return m_thread; }

private:
    CTestWindow m_testWnd;
    int m_thread;
};


#endif  //MYTHREAD_H
