///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class


#ifndef MAINFRM_H
#define MAINFRM_H

#include "../WinDev++/Frame.h"
#include "View.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
};

#endif //MAINFRM_H

