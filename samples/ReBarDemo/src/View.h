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
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView(const CView&);                // Disable copy construction
    CView& operator = (const CView&);   // Disable assignment operator
};

#endif // VIEW_H
