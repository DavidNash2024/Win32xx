#ifndef VIEW_H
#define VIEW_H


#include "..\Win32++\Wincore.h"
#include "..\Win32++\WCEframe.h"
#include "..\Win32++\Dialog.h"
#include <vector>


class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame() {}
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual void SetPen(COLORREF Color);

protected:
	virtual void DrawLine(int x, int y);
	virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(HDC hDC);
	virtual void StorePoint(int x, int y, bool PenDown);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	struct PlotPoint
	{
		int x;
		int y;
		bool PenDown;
		COLORREF color;
	};

	std::vector<PlotPoint> m_points;	// Points of lines to draw
	COLORREF m_PenColor;
};

#endif   //VIEW_H
