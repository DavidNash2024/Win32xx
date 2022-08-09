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
    virtual ~CView(){}

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView(const CView&);                // Disable copy construction
    CView& operator = (const CView&);   // Disable assignment operator

    void RecalcLayout();
    void SetWrapState(bool isWrapped);

    // Command handlers
    BOOL OnBottom();
    BOOL OnLeft();
    BOOL OnRight();
    BOOL OnTop();

    CToolBar m_toolBar;
    CImageList m_toolBarImages;
};


#endif // VIEW_H
