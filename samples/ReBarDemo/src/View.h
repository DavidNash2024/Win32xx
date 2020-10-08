/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H


/////////////////////////////////////////////
// CView manages CMainFrame's view window.
// This view also has a movable rebar window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView(){}

protected:
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMenuBar m_menuBar;
    CReBar m_reBar;
    CToolBar m_toolBar;
    CImageList m_toolBarImages;
};

#endif // VIEW_H
