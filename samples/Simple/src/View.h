/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


#include "wincore.h"


class CView : public CWnd
{
public:
	CView() {}
	virtual ~CView() {}

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnDraw(CDC* pDC);	
	virtual void OnInitialUpdate();
	virtual void OnSize();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif
