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
// CView is the application's main window.
// In this view, we use the mouse to draw on the
// window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView();

protected:
    virtual void OnDestroy();
    virtual void OnDraw(CDC& dc);
    virtual LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void DrawLine(int x, int y);
    void StorePoint(int x, int y, bool isPenDown);
    std::vector<PlotPoint> m_points;    // Points of lines to draw  COLORREF m_PenColor;
};


#endif // CVIEW_H
