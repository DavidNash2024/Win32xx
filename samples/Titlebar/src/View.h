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
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView(const CView&) = delete;                // Disable copy construction
    CView& operator = (const CView&) = delete;   // Disable assignment operator
};


#endif // VIEW_H
