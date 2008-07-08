#ifndef VIEW_H
#define VIEW_H

#include "../DevWin++/Wincore.h"


class CView : public CWnd
{
public:
	CView() {}
	virtual ~CView() {}
	virtual void OnPaint(HDC hDC);
	virtual	LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif   //VIEW_H
