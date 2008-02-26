//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class


#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include "../Win32++/WinCore.h"

using std::vector;


class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void PreRegisterClass(WNDCLASS &wc);
	virtual void SetPen(COLORREF  Color);
		
protected:
	virtual void DrawLine(int x, int y);
	virtual void OnPaint(HDC hDC);
	virtual void StorePoint(int x, int y, bool PenDown);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	struct PlotPoint
	{
		int x;
		int y;
		bool PenDown;
		COLORREF color;
	};

	HBRUSH m_hBrush;
	vector<PlotPoint> m_points;	// Points of lines to draw
	COLORREF m_PenColor;
};


#endif // CVIEW_H
