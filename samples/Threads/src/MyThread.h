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
    CMyThread(int threadNumber);
    virtual ~CMyThread() {}

    void CloseThread();
    CTestWindow* GetTestWnd()   { return &m_testWnd; }
    int GetThreadNumber()       { return m_threadNumber; }

protected:
    virtual BOOL InitInstance();

private:
    CTestWindow m_testWnd;
    int m_threadNumber;
};


#endif  //MYTHREAD_H
