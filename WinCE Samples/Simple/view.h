#ifndef VIEW_H
#define VIEW_H

#include "../Win32++/wincore.h"


class CView : public CWnd
{
public:
	CView() {}
	virtual ~CView() {}
	virtual void OnPaint(HDC hDC);
	virtual	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif   //VIEW_H
