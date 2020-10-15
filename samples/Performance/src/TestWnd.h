/////////////////////////////
// TestWnd.h
//

#ifndef TESTWND_H
#define TESTWND_H


///////////////////////////////////////////////////////////
// CTestWindow manages the test windows.
// Several test windows are created. Each of these are sent
// messages by the main window.
class CTestWindow : public CWnd
{
public:
    CTestWindow();
    virtual ~CTestWindow(){}
    void CreateWin(int i);

protected:
    virtual void OnInitialUpdate();
    virtual LRESULT WndProc(UINT uMsg, WPARAM wparam, LPARAM lparam);

private:
    enum Constants
    {
        WM_WINDOWCREATED     = WM_USER+1,
        WM_TESTMESSAGE       = WM_USER+2
    };

    int m_windowCount;
};


#endif  //TESTWIN_H
