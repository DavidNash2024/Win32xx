/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include "Doc.h"


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView() override = default;
    void SetPenColor(COLORREF Color) { m_penColor = Color; }

protected:
    virtual int OnCreate(CREATESTRUCT&) override;
    virtual void OnDraw(CDC& dc) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&);               // Disable copy construction
    CView& operator=(const CView&);    // Disable assignment operator

    // Message handlers
    LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);

    void DrawLine(int x, int y);
    std::vector<PlotPoint>* GetAllPoints();
    COLORREF GetPenColor() { return m_penColor; }
    void SendPoint(int x, int y, bool isPenDown);

    // Member variables
    CBrush m_brush;
    COLORREF m_penColor;
};


#endif // CVIEW_H
