//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H


class CDoc;


struct PlotPoint
{
	int x;
	int y;
	bool PenDown;
	COLORREF color;
};


class CView : public CWnd
{
public:
	CView();
	virtual ~CView();

	CDoc& GetDoc();
	std::vector<PlotPoint>& GetPoints();
	COLORREF GetPenColor() { return m_PenColor; }
	void SetPenColor(COLORREF Color) { m_PenColor = Color; }

protected:
	virtual void OnDraw(CDC& dc);
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
