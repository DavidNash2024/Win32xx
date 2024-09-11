/////////////////////////////
// Rect.h
//

#ifndef RECT_H
#define RECT_H

struct RectData
{
    RectData(COLORREF clr, CRect rc) : color(clr), rect(rc) {}
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
    virtual ~CViewRect() override {}

protected:
    // Virtual functions that override base class functions
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnDestroy() override;
    virtual void    OnDraw(CDC& dc) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewRect(const CViewRect&) = delete;
    CViewRect& operator=(const CViewRect&) = delete;

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
