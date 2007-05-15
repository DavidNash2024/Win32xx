//////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class


#ifndef MAINFRM_H
#define MAINFRM_H

#include "view.h"
#include "..\Win32++\frame.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	HBITMAP m_hBitmap;

};

#endif //MAINFRM_H
