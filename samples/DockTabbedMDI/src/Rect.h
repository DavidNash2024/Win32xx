/////////////////////////////
// Rect.h
//

#ifndef RECT_H
#define RECT_H

struct RectData
{
    COLORREF color;
    CRect rect;
};

//////////////////////////////////////////////////////////////
// CViewRect manages a window that displays random rectangles.
// This a view window used by CMyTabbedMDI.
class CViewRect : public CWnd
{
public:
    CViewRect() : m_cxClientMax(0), m_cyClientMax(0) {}
    virtual ~CViewRect() {}

// Set the CREATESTURCT parameters before the window is created.
void PreCreate(CREATESTRUCT& cs)
{
    CWnd::PreCreate(cs);
    cs.style |= WS_CLIPCHILDREN;
}

protected:
    // Virtual functions that override base class functions
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnDestroy();
    virtual void    OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewRect(const CViewRect&);               // Disable copy construction
    CViewRect& operator=(const CViewRect&);    // Disable assignment operator

    // Command handlers
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnTimer(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    std::vector<RectData> m_rects;
    int m_cxClientMax;
    int m_cyClientMax;
};

#endif  //RECT_H
