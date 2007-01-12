/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

#include "..\Win32++\Wincore.h"


class CView : public CWnd
{
public:
	CView() {}
	virtual ~CView() {}

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual void OnSize();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif
