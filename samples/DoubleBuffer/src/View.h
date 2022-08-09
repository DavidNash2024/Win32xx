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

protected:
    // Virtual functions that override base class functions
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual void    PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView(const CView&);                // Disable copy construction
    CView& operator = (const CView&);   // Disable assignment operator

    // Command handlers
    LRESULT OnTimer(UINT msg, WPARAM wparam, LPARAM lparam);

    CBitmap CreateMaskBitmap();

    // Member variables
    CBitmap m_blue;
    CBitmap m_orange;
    CBitmap m_mask;
    CSize m_ballSize;
};


#endif // VIEW_H
