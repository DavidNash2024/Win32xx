/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CWnd
{
public:
    CView() = default;
    virtual ~CView() override = default;

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc) override;
    virtual void OnInitialUpdate() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;
};


#endif // VIEW_H
