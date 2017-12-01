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

protected:
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CMenuBar m_MenuBar;
    CReBar m_ReBar;
    CToolBar m_ToolBar;
    CImageList m_ToolBarImages;
};


#endif // VIEW_H
