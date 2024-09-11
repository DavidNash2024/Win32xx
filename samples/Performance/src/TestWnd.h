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
    virtual ~CTestWindow() override {}
    void CreateWin(int i);

protected:
    // Virtual functions that override base class functions
    virtual int OnCreate(CREATESTRUCT& cs) override;
    virtual void OnInitialUpdate() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CTestWindow(const CTestWindow&) = delete;
    CTestWindow& operator=(const CTestWindow&) = delete;

    // Message handlers
    LRESULT OnTestMessage();

    int m_windowCount;
};


#endif  //TESTWIN_H
