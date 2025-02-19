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
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnInitialUpdate() override;
    virtual void    PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

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
