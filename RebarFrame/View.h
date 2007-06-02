//////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H

#include "../Win32++/WinCore.h"


class CView : public CWnd
{
public:
	CView();
	virtual ~CView(){}

protected:
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void OnPaint(HDC hDC);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif // VIEW_H
