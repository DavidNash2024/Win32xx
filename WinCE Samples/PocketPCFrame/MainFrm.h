#ifndef MAINFRM_H
#define MAINFRM_H


#include "..\Win32++\Wincore.h"
#include "..\Win32++\WCEframe.h"
#include "..\Win32++\Dialog.h"


class CMainFrame : public CFrame
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
