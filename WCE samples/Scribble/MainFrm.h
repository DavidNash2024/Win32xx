/////////////////////////////
// MainFrm.h


#ifndef MAINFRM_H_
#define MAINFRM_H_


#include "wxx_wincore.h"
#include "wxx_wceframe.h"
#include "wxx_dialog.h"
#include <vector>


////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
class CMainFrame : public CWceFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() {}
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnInitialUpdate();
    virtual void SetPen(COLORREF Color);

protected:
    virtual void DrawLine(short x, short y);
    virtual void OnDraw(CDC& dc);
    virtual LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void StorePoint(int x, int y, bool PenDown);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    struct PlotPoint
    {
        short x;
        short y;
        bool isPenDown;
        COLORREF penColor;
    };

    std::vector<PlotPoint> m_points;    // Points of lines to draw
    COLORREF m_penColor;
};


#endif   // MAINFRM_H_
