#ifndef MAINFRM_H
#define MAINFRM_H


#include "../WinDev++/Wincore.h"
#include "../WinDev++/WCEframe.h"
#include "../WinDev++/Dialog.h"


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
