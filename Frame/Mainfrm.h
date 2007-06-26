/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../Win32++/frame.h"
#include "View.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
//	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetButtons(const std::vector<UINT> ToolbarData);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
};

#endif //MAINFRM_H

