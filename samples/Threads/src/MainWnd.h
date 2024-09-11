/////////////////////////////
// MainWnd.h
//

#ifndef MAINWND_H
#define MAINWND_H

#include "TestWnd.h"
#include "MyThread.h"
#include "MyEdit.h"

using MyThreadPtr = std::unique_ptr<CMyWinThread>;


/////////////////////////////////////////////////////
// CMainWindow manages the application's main window.
class CMainWindow : public CWnd
{
public:
    CMainWindow();
    virtual ~CMainWindow() override {}
    HWND Create(HWND parent = nullptr) override;

protected:
    // Virtual functions that override base class functions
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnDestroy() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainWindow(const CMainWindow&) = delete;
    CMainWindow& operator=(const CMainWindow&) = delete;

    void AppendText(LPCWSTR text);

    // Message handlers
    void    OnAllWindowsCreated();
    LRESULT OnAppendText(WPARAM wparam);
    LRESULT OnCloseThread(WPARAM wparam);
    LRESULT OnDpiChanged(UINT, WPARAM, LPARAM lparam);
    LRESULT OnSize();
    LRESULT OnWindowCreated();

    CCriticalSection m_cs;              // Used to ensure thread safe access to m_threads.
    CMyEdit m_edit;                     // Child window edit control.
    std::vector<MyThreadPtr> m_threads; // A vector of CMyWinThread smart pointers.
    int m_maxWindows;                   // Number of additional test windows to be created.
    int m_windowsCount;                 // Count of windows actually created.
};


#endif  //MAINWND_H
