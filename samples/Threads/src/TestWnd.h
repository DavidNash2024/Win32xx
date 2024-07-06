/////////////////////////////
// TestWnd.h
//

#ifndef TESTWND_H
#define TESTWND_H


////////////////////////////////////////////////////////////
// CTestWindow manages the test window created by CMyWinThread.
class CTestWindow : public CWnd
{
public:
    CTestWindow(int thread, HWND mainWindow);
    virtual ~CTestWindow() {}
    virtual HWND Create(HWND parent = NULL);

protected:
    // Virtual functions that override base class functions
    virtual void OnClose();
    virtual void OnInitialUpdate();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CTestWindow(const CTestWindow&);               // Disable copy construction
    CTestWindow& operator=(const CTestWindow&);    // Disable assignment operator

    int  m_threadNumber;
    HWND m_mainWindow;
};


#endif  //TESTWIN_H
