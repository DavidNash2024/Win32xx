//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H


#include "wxx_wincore.h"
#include <vector>



struct PlotPoint
{
	int x;
	int y;
	bool PenDown;
};


class CView : public CWnd
{
public:
	CView();
	virtual ~CView();

protected:
	virtual void OnDraw(CDC& dc);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void DrawLine(int x, int y);
	void StorePoint(int x, int y, bool PenDown);
	std::vector<PlotPoint> m_points;	// Points of lines to draw	COLORREF m_PenColor;
};


#endif // CVIEW_H
