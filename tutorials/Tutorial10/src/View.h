//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H

#include "targetver.h"
#include "wxx_wincore.h"
#include "Doc.h"


// Message - sent to the parent (Frame) window when a file is dropped on the View window
//   WPARAM: A pointer to the filename (LPCTSTR)
//   LPARAM: unused
#define UWM_DROPFILE (WM_APP + 0x0001)  


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
    void SetPenColor(COLORREF Color) { m_penColor = Color; }

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
    void DrawLine(int x, int y);

    CDoc   m_doc;
    CBrush m_brush;
    COLORREF m_penColor;
};


#endif // CVIEW_H
