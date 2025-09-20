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
    virtual ~CTestWindow() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    virtual void OnClose() override;
    virtual void OnInitialUpdate() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CTestWindow(const CTestWindow&) = delete;
    CTestWindow& operator=(const CTestWindow&) = delete;

    int  m_threadNumber;
    HWND m_mainWindow;
};


#endif  //TESTWIN_H
