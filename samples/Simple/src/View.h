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
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnDestroy();
	virtual void OnDraw(CDC& dc);	
	virtual void OnInitialUpdate();
	virtual void OnSize();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif
