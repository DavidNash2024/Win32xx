/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H


///////////////////////////////////////////////
// CView manages CMainFrame's view window.
// This view also has a movable toolbar window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView() override {}

    void DpiScaleToolBar();

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnDraw(CDC& dc) override;
    virtual void OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    void RecalcLayout();
    void SetWrapState(bool isWrapped);

    // MessageHandlers
    LRESULT OnSize(UINT, WPARAM, LPARAM);
    LRESULT OnDpiChangedBeforeParent(UINT, WPARAM, LPARAM);

    // Command handlers
    BOOL OnBottom();
    BOOL OnLeft();
    BOOL OnRight();
    BOOL OnTop();

    CToolBar m_toolBar;
    CImageList m_toolBarImages;
};


#endif // VIEW_H
