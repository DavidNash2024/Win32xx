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
	int m_x;
	int m_y;
	bool m_PenDown;
	COLORREF m_color;

	// The constructors
	PlotPoint() : m_x(0), m_y(0), m_PenDown(false), m_color(RGB(0, 0, 0)) {}
	
	PlotPoint(int x, int y, bool PenDown, COLORREF color) :
		m_x(x), m_y(y), m_PenDown(PenDown), m_color(color) {}

};


class CView : public CWnd
{
public:
	CView();
	virtual ~CView();

	std::vector<PlotPoint>* GetAllPoints();
	COLORREF GetPenColor() { return m_PenColor; }
	void SetPenColor(COLORREF Color) { m_PenColor = Color; }
	void SendPoint(int x, int y, bool PenDown);

protected:
	virtual int OnCreate(CREATESTRUCT&);
	virtual void OnDraw(CDC& dc);
	virtual LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual void PreRegisterClass(WNDCLASS& wc);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void DrawLine(int x, int y);

	CBrush m_Brush;
	COLORREF m_PenColor;

};


#endif // CVIEW_H
