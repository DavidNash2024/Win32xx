/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include "Doc.h"

// Message - sent to the parent (Frame) window when a file is dropped on the View window.
//   WPARAM: A pointer to the filename (LPCWSTR).
//   LPARAM: unused.
#define UWM_DROPFILE (WM_APP + 0x0001)


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView() override = default;
    CDoc& GetDoc();
    std::vector<PlotPoint>& GetAllPoints();
    COLORREF GetPenColor() { return m_penColor; }
    void Print();
    void SetPenColor(COLORREF color) { m_penColor = color; }

protected:
    virtual int OnCreate(CREATESTRUCT&) override;
    virtual void OnDraw(CDC& dc) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    // Message handlers.
    LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);

    void DrawLine(int x, int y);

    // Member variables.
    CDoc m_doc;
    CBrush m_brush;
    COLORREF m_penColor;
};


#endif // CVIEW_H
