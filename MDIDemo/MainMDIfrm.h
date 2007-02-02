///////////////////////////////////////////////////////
// MainMDIfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H


#include "..\Win32++\MDI.h"


class CMainMDIFrame : public CMDIFrame
{
public:
	CMainMDIFrame(void);
	virtual ~CMainMDIFrame();

protected:
	virtual BOOL OnCommand(UINT nID);
	virtual void OnInitialUpdate();
	virtual void SetButtons(std::vector<UINT> ToolbarData);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif  //MAINMDIFRM_H

