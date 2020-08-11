//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H


// Message - sent to the parent (Frame) window when a file is dropped on the View window
//   WPARAM: A pointer to the filename (LPCTSTR)
//   LPARAM: unused
#define UWM_DROPFILE (WM_APP + 0x0001)

// Message - sent to the parent (Frame) window to retrieve the PlotPoint data.
//   Returns a pointer to a vector of PlotPoint
#define UWN_GETALLPOINTS (WM_APP + 0x0002)

// Message - sent to the parent (Frame) window containing the PlotPoint information.
//   WPARAM: A pointer to PlotPoint.
//   LPARAM: unused
#define UWM_SENDPOINT (WM_APP + 0x0003)


// The PlotPoint struct holds the drawing information for each point
struct PlotPoint
{
    int x;
    int y;
    bool isPenDown;
    COLORREF color;
};


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
