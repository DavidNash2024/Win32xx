////////////////////////////////////////
// MainWnd.h
//  Declaration of the CMainWindow class

#ifndef MAINWND_H
#define MAINWND_H

#include "TestWnd.h"
#include "MyThread.h"
#include "MyEdit.h"

typedef Shared_Ptr<CMyThread> MyThreadPtr;


class CMainWindow : public CWnd
{
public:
    CMainWindow();
    virtual ~CMainWindow() {}
    virtual void AppendText(LPCTSTR text);
    virtual HWND Create(HWND parent = 0);
    virtual void OnAllWindowsCreated();
    virtual void OnWindowCreated();

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

    std::vector<MyThreadPtr> m_threads;// A vectore of CMyThread smart pointers
    int m_maxWindows;                // Number of additional test windows to be created
    int m_windowsCount;              // Count of windows actually created
    CMyEdit m_edit;
};


#endif  //MAINWND_H
