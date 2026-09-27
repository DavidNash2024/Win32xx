/////////////////////////////
// MyThread.h
//

#pragma once

#include "TestWnd.h"

/////////////////////////////////////////////////////////
// CMyWinThread manages a windows thread. It runs a separate
// message loop and creates a test window when it starts.
class CMyWinThread : public CWinThread
{
public:
    CMyWinThread(int threadNumber, HWND mainWindow);
    virtual ~CMyWinThread() override;
    int GetThreadNumber()     { return m_threadNumber; }
    CTestWindow* GetTestWnd() { return &m_testWnd; }

protected:
    virtual BOOL InitInstance() override;

private:
    CMyWinThread(const CMyWinThread&) = delete;
    CMyWinThread& operator=(const CMyWinThread&) = delete;

    void CloseThread();

    CTestWindow m_testWnd;
    int         m_threadNumber;
    HWND        m_mainWindow;
};

