/////////////////////////////
// TestWindow.h
//

#ifndef TESTWINDOW_H
#define TESTWINDOW_H


class CTestWindow : public CWnd
{
public:
    CTestWindow() : m_menu(IDW_MAIN) {}
    virtual ~CTestWindow() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnDestroy() override;
    virtual void OnDraw(CDC& dc) override;
    virtual void OnInitialUpdate() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CTestWindow(const CTestWindow&) = delete;
    CTestWindow& operator=(const CTestWindow&) = delete;

    // Message handlers
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);

    BOOL OnFileExit();
    BOOL OnViewDialog();

    // Member variables
    CMenu m_menu;
};

#endif  // TESTWINDOW_H
