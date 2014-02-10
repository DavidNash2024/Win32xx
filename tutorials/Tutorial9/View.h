//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H

#include "targetver.h"
#include "wincore.h"
#include <vector>

using std::vector;


// A class that inherits from CWnd. It is used to create the window.
class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	virtual void ClearPoints();
	virtual BOOL FileOpen(LPCTSTR szFilename);
	virtual BOOL FileSave(LPCTSTR szFilename);
	virtual void SetPen(COLORREF Color);

protected:
	virtual void OnDraw(CDC* pDC);
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
	LRESULT OnLButtonDown(LPARAM lParam);
	LRESULT OnLButtonUp(LPARAM lParam);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	void StorePoint(int x, int y, bool PenDown);

	CBrush m_Brush;
	vector<PlotPoint> m_points;	// Points of lines to draw
	COLORREF m_PenColor;
};


#endif // CVIEW_H
