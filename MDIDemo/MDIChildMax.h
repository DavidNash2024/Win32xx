//////////////////////////////////////////////////////
// MDIChildMax.h
//  Declaration of the CMDIChildMax class


#ifndef MDICHILDMAX_H
#define MDICHILDMAX_H

#include "../DevWin++/MDI.h"


class CMDIChildMax : public CMDIChild
{
public:
	CMDIChildMax();
	virtual ~CMDIChildMax();

protected:
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif  //MDICHILDMAX_H
