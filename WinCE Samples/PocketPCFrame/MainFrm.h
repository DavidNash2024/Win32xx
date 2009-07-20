#ifndef MAINFRM_H
#define MAINFRM_H


#include "../Win32++/wincore.h"
#include "../Win32++/wceframe.h"
#include "../Win32++/dialog.h"


class CMainFrame : public CWceFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif   // MAINFRM_H
