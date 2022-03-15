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
    void SetPenColor(COLORREF Color) { m_penColor = Color; }

protected:
    // Virtual functions that override base class functions
    virtual int OnCreate(CREATESTRUCT&);
    virtual void OnDraw(CDC& dc);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
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
