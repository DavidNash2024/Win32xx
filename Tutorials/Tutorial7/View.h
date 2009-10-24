//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H


#include "WinCore.h"
#include <vector>

using std::vector;


class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	virtual void SetPen(COLORREF  Color);

protected:
	virtual void OnPaint(HDC hDC);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void PreRegisterClass(WNDCLASS &wc);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	struct PlotPoint
	{
		int x;
		int y;
		bool PenDown;
		COLORREF color;
	};

	void DrawLine(int x, int y);
	void OnLButtonDown(LPARAM lParam);
	void OnLButtonUp(LPARAM lParam);
	void OnMouseMove(WPARAM wParam, LPARAM lParam);
	void StorePoint(int x, int y, bool PenDown);

	HBRUSH m_hBrush;
	vector<PlotPoint> m_points;	// Points of lines to draw
	COLORREF m_PenColor;
};


#endif // CVIEW_H
