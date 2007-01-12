///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class


#ifndef MAINFRM_H
#define MAINFRM_H

#include "..\Win32++\frame.h"
#include "View.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnCommand(UINT nID);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	enum Pens
	{
		RED, BLUE, GREEN, BLACK
	};

	CView m_View;

};

#endif //MAINFRM_H

