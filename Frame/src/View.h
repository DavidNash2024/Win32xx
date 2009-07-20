//////////////////////////////////////////////////////
// View.h


#ifndef VIEW_H
#define VIEW_H

#include "../../Win32++/wincore.h"


// Declaration of the CView class
class CView : public CWnd
{
public:
	CView();
	virtual ~CView(){}

protected:
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif // VIEW_H
