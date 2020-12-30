/////////////////////////////
// MainWnd.h
//

#ifndef MAINWND_H
#define MAINWND_H

#include "TestWnd.h"
#include "MyThread.h"
#include "MyEdit.h"

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
    void OnWindowCreated();

protected:
    virtual void OnClose();
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    enum Constants
    {
        WM_WINDOWCREATED = WM_USER+1,   // the message sent when window is created
        WM_TESTMESSAGE   = WM_USER+2    // the test message
    };

    std::vector<MyThreadPtr> m_threads; // A vector of CMyThread smart pointers
    int m_maxWindows;                   // Number of additional test windows to be created
    int m_windowsCount;                 // Count of windows actually created
    CMyEdit m_edit;
};


#endif  //MAINWND_H
