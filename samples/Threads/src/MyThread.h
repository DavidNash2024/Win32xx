/////////////////////////////
// MyThread.h
//

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "TestWnd.h"


/////////////////////////////////////////////////////////
// CMyWinThread manages a windows thread. It runs a separate
// message loop and creates a test window when it starts.
class CMyWinThread : public CWinThread
{
public:
    CMyWinThread(int threadNumber, HWND mainWindow);
    virtual ~CMyWinThread();
    int GetThreadNumber() { return m_threadNumber; }

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    void CloseThread();
    CTestWindow* GetTestWnd() { return &m_testWnd; }

    CTestWindow m_testWnd;
    int         m_threadNumber;
    HWND        m_mainWindow;
};


#endif  //MYTHREAD_H
