//////////////////////////////////////////////////////
// MDIChildView.h
//  Declaration of the CMDIChildView class


#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H

#include "../Win32++/MDI.h"


class CMDIChildView : public CMDIChild
{
public:
	CMDIChildView();
	virtual ~CMDIChildView();

protected:
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	COLORREF m_Color;
};

#endif  //MDICHILDVIEW_H
