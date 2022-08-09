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
    // Virtual functions that override base class functions
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CTestWindow(const CTestWindow&);                // Disable copy construction
    CTestWindow& operator = (const CTestWindow&);   // Disable assignment operator

    // Message handlers
    LRESULT OnTestMessage();

    int m_windowCount;
};


#endif  //TESTWIN_H
