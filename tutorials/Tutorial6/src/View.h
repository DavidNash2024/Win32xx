//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H


#include "wxx_wincore.h"
#include <vector>


struct PlotPoint
{
    int x;
    int y;
    bool isPenDown;
};


////////////////////////////////////////////////
// CView manages CMainFrame's view window.
// In this view, we use the mouse to draw on the
// window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView() override = default;

    void DrawLine(int x, int y);
    void StorePoint(int x, int y, bool isPenDown);

protected:
    virtual void OnDraw(CDC& dc) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);

    CBrush m_brush;
    std::vector<PlotPoint> m_points;    // Points of lines to draw  COLORREF m_PenColor.
};


#endif // CVIEW_H
