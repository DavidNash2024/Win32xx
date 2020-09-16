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
    CView();
    virtual ~CView(){}

    CBitmap CreateMaskBitmap();

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnTimer(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CBitmap m_blue;
    CBitmap m_orange;
    CBitmap m_mask;
    CSize m_ballSize;
};


#endif // VIEW_H
