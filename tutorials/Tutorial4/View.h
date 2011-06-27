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
	virtual ~CView(){}

protected:
	virtual void OnDraw(CDC& dc);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	struct PlotPoint
	{
		int x;
		int y;
		bool PenDown;
	};

	void DrawLine(int x, int y);
	void OnLButtonDown(LPARAM lParam);
	void OnLButtonUp(LPARAM lParam);
	void OnMouseMove(WPARAM wParam, LPARAM lParam);
	void StorePoint(int x, int y, bool PenDown);

	vector<PlotPoint> m_points;	// Points of lines to draw
};


#endif // CVIEW_H
