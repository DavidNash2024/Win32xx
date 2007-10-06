///////////////////////////////////////////////////////
// MainMDIfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H


#include "../Win32++/MDI.h"


class CMainMDIFrame : public CMDIFrame
{
public:
	CMainMDIFrame(void);
	virtual ~CMainMDIFrame();

protected:
	virtual void DoPopupMenu();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM /*wParam*/, LPARAM lParam);
	virtual void SetButtons(const std::vector<UINT> ToolbarData);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif  //MAINMDIFRM_H

