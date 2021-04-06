/////////////////////////////
// MainWnd.h
//

#ifndef MAINWND_H
#define MAINWND_H

#include "TestWnd.h"
#include "MyThread.h"
#include "MyEdit.h"

// Note: Modern C++ compilers can use this typedef instead.
// typedef std::shared_ptr<CMyThread> MyThreadPtr;
typedef Shared_Ptr<CMyThread> MyThreadPtr;


/////////////////////////////////////////////////////
// CMainWindow manages the application's main window.
class CMainWindow : public CWnd
{
public:
    CMainWindow();
    virtual ~CMainWindow() {}
    void AppendText(LPCTSTR text);
    HWND Create(HWND parent = 0);
    void OnAllWindowsCreated();
    void OnAppendText(WPARAM wparam);
    void OnCloseThread(WPARAM wparam);
    void OnSize();
    void OnWindowCreated();

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CCriticalSection m_cs;              // Used to ensure thread safe access to m_threads.
    CMyEdit m_edit;                     // Child window edit control.
    std::vector<MyThreadPtr> m_threads; // A vector of CMyThread smart pointers.
    int m_maxWindows;                   // Number of additional test windows to be created.
    int m_windowsCount;                 // Count of windows actually created.
};


#endif  //MAINWND_H
