//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H


#include "targetver.h"
#include "wxx_wincore.h"
#include <vector>


struct PlotPoint
{
    int x;
    int y;
    bool isPenDown;
    COLORREF penColor;
};


////////////////////////////////////////////////
// CView manages CMainFrame's view window.
// In this view, we use the mouse to draw on the
// window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView();

    COLORREF GetPenColor() { return m_penColor; }
    void SetPenColor(COLORREF color) { m_penColor = color; }

protected:
    virtual void OnDraw(CDC& dc);
    virtual LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void DrawLine(int x, int y);
    void StorePoint(int x, int y, bool isPenDown);
    CBrush m_brush;
    std::vector<PlotPoint> m_points;    // Points of lines to draw  COLORREF m_PenColor;
    COLORREF m_penColor;
};


#endif // CVIEW_H
