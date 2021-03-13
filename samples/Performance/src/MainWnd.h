/////////////////////////////
// MainWnd.h
//

#ifndef MAINWND_H
#define MAINWND_H


#include "MyEdit.h"
#include "TestWnd.h"

// Note: Modern C++ compilers can use this typedef instead.
// typedef std::shared_ptr<CTestWindow> TestWindowPtr;
typedef Shared_Ptr<CTestWindow> TestWindowPtr;

///////////////////////////////////////////////////////////
// CMainWindow manages the main window for the application.
class CMainWindow : public CWnd
{
public:
    CMainWindow();
    virtual ~CMainWindow();
    virtual HWND Create(HWND hParent = 0);
    virtual void CreateTestWindows(int windows);
    virtual void OnAllWindowsCreated();
    virtual void PerformanceTest();
    virtual void SendText(LPCTSTR str);
    virtual void SetTestMessages(int testMessages) {m_testMessages = testMessages;}

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual void OnInitialUpdate();
    virtual LRESULT OnSize();
    virtual LRESULT OnWindowCreated();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    enum Constants
    {
        WM_WINDOWCREATED     = WM_USER+1,   // the message sent when window is created
        WM_TESTMESSAGE       = WM_USER+2    // the test message
    };

    std::vector<TestWindowPtr> m_pTestWindows; // A vector CTestWindow smart pointers
    CMyEdit m_edit;        // Handle to the edit window
    int m_testMessages;    // Number of test messages to be sent
    int m_testWindows;     // Number of test windows to create
    int m_windowsCreated;  // Number of windows created
};


#endif  //MAINWND_H
