//////////////////////////////////////
// MyThread.h
//  Declaration of the CMyThread class

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "TestWnd.h"


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
