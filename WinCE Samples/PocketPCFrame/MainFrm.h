#ifndef MAINFRM_H
#define MAINFRM_H


#include "../DevWin++/Wincore.h"
#include "../DevWin++/WCEframe.h"
#include "../DevWin++/Dialog.h"


class CMainFrame : public CWceFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif   // MAINFRM_H
