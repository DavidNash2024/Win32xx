//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

class CDoc;

// Declaration of the CView class
class CView : public CWnd
{
public:
    CView();
    virtual ~CView(){}
    CDoc& GetDoc();

protected:
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif // VIEW_H
