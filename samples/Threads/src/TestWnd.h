/////////////////////////////
// TestWnd.h
//

#ifndef TESTWND_H
#define TESTWND_H


////////////////////////////////////////////////////////////
// CTestWindow manages the test window created by CMyThread.
class CTestWindow : public CWnd
{
public:
    CTestWindow(int thread, HWND mainWindow);
    virtual ~CTestWindow() {}
    virtual HWND Create(HWND parent = 0);

protected:
    virtual void OnClose();
    virtual void OnInitialUpdate();

private:
    int  m_threadNumber;
    HWND m_mainWindow;
};


#endif  //TESTWIN_H
