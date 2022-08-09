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
    virtual HWND Create(HWND parent = 0);

protected:
    // Virtual functions that override base class functions
    virtual void OnClose();
    virtual void OnInitialUpdate();

private:
    CTestWindow(const CTestWindow&);                // Disable copy construction
    CTestWindow& operator = (const CTestWindow&);   // Disable assignment operator

    int  m_threadNumber;
    HWND m_mainWindow;
};


#endif  //TESTWIN_H
