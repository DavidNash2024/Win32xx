/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

#include "../Win32++/Wincore.h"
#include <Windowsx.h>
#include "TabControl.h"

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
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CTabCtrl m_MyTab;
	CWnd m_StaticWnd;
};


#endif
