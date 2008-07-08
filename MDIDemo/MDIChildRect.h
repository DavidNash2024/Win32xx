//////////////////////////////////////////////////////
// MDIChildRect.h
//  Declaration of the CMDIChildRect class


#ifndef MDICHILDRECT_H
#define MDICHILDRECT_H

#include "../DevWin++/MDI.h"


class CMDIChildRect : public CMDIChild
{
public:
	CMDIChildRect();
	virtual ~CMDIChildRect();

protected:
	virtual void OnInitialUpdate();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_cxClientMax;
	int m_cyClientMax;
};


#endif  //MDICHILDRECT_H
