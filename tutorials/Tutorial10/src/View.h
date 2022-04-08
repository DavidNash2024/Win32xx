//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H

#include "Doc.h"


// Message - sent to the parent (Frame) window when a file is dropped on the View window.
//   WPARAM: A pointer to the filename (LPCTSTR)
//   LPARAM: unused
#define UWM_DROPFILE (WM_APP + 0x0001)


////////////////////////////////////////////////
// CView manages CMainFrame's view window.
// In this view, we use the mouse to draw on the
// window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView();

    CDoc& GetDoc();
    std::vector<PlotPoint>& GetAllPoints();
    COLORREF GetPenColor() { return m_penColor; }
    void Print(LPCTSTR docName);
    void PrintPage(CDC& dc, UINT page = 1);
    void QuickPrint(LPCTSTR docName);
    void SetPenColor(COLORREF color) { m_penColor = color; }

protected:
    virtual int OnCreate(CREATESTRUCT&);
    virtual void OnDraw(CDC& dc);
    virtual LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMemDC Draw();
    void DrawLine(int x, int y);

    CDoc m_doc;
    CBrush m_brush;
    COLORREF m_penColor;
};


#endif // CVIEW_H
