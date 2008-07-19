///////////////////////////////////////////////////////
// DockFrame.h
//  Declaration of the CDockFrame class

#ifndef DOCKFRAME_H
#define DOCKFRAME_H


#include "../Win32++/MDI.h"
#include "Dockable.h"


// Docking Styles
#define DS_DOCK_LEFT		0x0001
#define DS_DOCK_RIGHT		0x0002
#define DS_DOCK_TOP			0x0004
#define DS_DOCK_BOTTOM		0x0008


class CDockFrame : public CMDIFrame
{
public:
	CDockFrame(void);
	virtual ~CDockFrame();
	UINT GetDockSide(LPDRAGPOS pdp);
	void Dock();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void RecalcLayout();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CDockable m_DockContainer;
};

#endif  // DOCKFRAME_H

