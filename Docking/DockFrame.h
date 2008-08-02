///////////////////////////////////////////////////////
// DockFrame.h
//  Declaration of the CDockFrame class

#ifndef DOCKFRAME_H
#define DOCKFRAME_H


#include "../Win32++/MDI.h"
#include "Dockable.h"


// Docking States
#define DS_DOCKED_LEFT		0x0001
#define DS_DOCKED_RIGHT		0x0002
#define DS_DOCKED_TOP		0x0004
#define DS_DOCKED_BOTTOM	0x0008



class CDockFrame : public CMDIFrame
{
public:
	CDockFrame(void);
	virtual ~CDockFrame();
	CDockable* AddDockable(CDockable* pDockable, UINT uDockSide, int DockWidth, CDockable* pDockParent = NULL);
//	CDockable* GetDockNeighbour(CDockable* pDock);
	UINT GetDockSide(LPDRAGPOS pdp);
	virtual void Dock(HWND hDockable, UINT DockState, CDockable* pDockParent = NULL);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual CRect RecalcDockLayout(CRect rc, CDockable* pDockParent);
	virtual void RecalcLayout();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void DrawHashBar(HWND hBar, POINT Pos);

	std::vector< CDockable* > m_vDockables;	
	std::vector< CBar* > m_vBars;
	HBITMAP	m_hbm;
	HBRUSH m_hbrDithered;
	int m_BarWidth;
	BOOL m_IsInDockZone;
	BOOL m_IsDraggingDockable;
	HDWP m_hDwp;
};



#endif  // DOCKFRAME_H

