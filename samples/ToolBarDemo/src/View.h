//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


// Declaration of the CView class
class CView : public CWnd
{
public:
    CView();
    virtual ~CView(){}

    void RecalcLayout();
    void SetWrapState(BOOL isWrapped);
    void OnBottom();
    void OnLeft();
    void OnRight();
    void OnTop();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CToolBar m_toolBar;
    CImageList m_toolBarImages;
};


#endif // VIEW_H
