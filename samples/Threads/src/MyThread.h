//////////////////////////////////////
// MyThread.h
//  Declaration of the CMyThread class

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "TestWnd.h"


class CMyThread : public CWinThread
{
public:
    CMyThread(int nThread);
    virtual ~CMyThread() {}
    
    virtual BOOL InitInstance();
    CTestWindow* GetTestWnd()   { return &m_TestWnd; }
    int GetThreadCount()        { return m_nThread; }

private:
    CTestWindow m_TestWnd;
    int m_nThread;
};


#endif  //MYTHREAD_H
