///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class


#ifndef MAINFRM_H
#define MAINFRM_H


#include <string>
#include "../Win32++/frame.h"
#include "Views.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CMainView m_MainView;
};

#endif //MAINFRM_H

