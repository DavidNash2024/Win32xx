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
    CTestWindow(int window);
    virtual ~CTestWindow() {}

    virtual HWND Create(HWND parent = 0);

protected:
    virtual void OnClose();
    virtual void OnDestroy();
    virtual void OnInitialUpdate();

private:
    enum Constants
    {
        WM_WINDOWCREATED     = WM_USER+1,
        WM_TESTMESSAGE       = WM_USER+2
    };

    int m_window;
};


#endif  //TESTWIN_H
