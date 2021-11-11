/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H


//////////////////////////////////////////
// CView manages CMiniFrame's view window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView(){}

protected:
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif // VIEW_H
