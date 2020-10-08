/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H


////////////////////////////////////////////////////////////
// Declaration of the PlotPoint struct.
// This struct holds the drawing information for each point.
struct PlotPoint
{
    int x;
    int y;
    bool isPenDown;
    COLORREF color;
};


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView();

    std::vector<PlotPoint>* GetAllPoints();
    COLORREF GetPenColor() { return m_penColor; }
    void SetPenColor(COLORREF Color) { m_penColor = Color; }
    void SendPoint(int x, int y, bool isPenDown);

protected:
    virtual int OnCreate(CREATESTRUCT&);
    virtual void OnDraw(CDC& dc);
    virtual LRESULT OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void DrawLine(int x, int y);

    CBrush m_brush;
    COLORREF m_penColor;
};


#endif // CVIEW_H
