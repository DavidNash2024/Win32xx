///////////////////////////////////////////////////////
// Dockable.h
//  Declaration of the CDockable class

#ifndef DOCKABLE_H
#define DOCKABLE_H


#include "../Win32++/WinCore.h"

enum Constants
{
	// Docking Notifications
	DN_DOCK_START= WM_APP + 1,	// Start dock window drag
	DN_DOCK_MOVE = WM_APP + 2,	// Dock window draging
	DN_DOCK_END  = WM_APP + 3,	// End dock window dragging
	DN_BAR_START = WM_APP + 4,	// Start splitter bar drag
	DN_BAR_MOVE  = WM_APP + 5,	// Splitter bar dragging
	DN_BAR_END   = WM_APP + 6   // End splitter bar drag
};

typedef struct DRAGPOS
{
    NMHDR hdr;
    POINT ptPos;
} *LPDRAGPOS;

class CDockable : public CWnd
{
public:
	CDockable();
	virtual ~CDockable() {}
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void SendNotify(UINT nMessageID);
	virtual void UnDock();

	// Attributes
	BOOL IsDocked() {return (BOOL)m_DockState;}
	UINT GetDockState() {return m_DockState;}
	int GetDockWidth() {return m_DockWidth;}
	void SetDockState(UINT uDockState) {m_DockState = uDockState;}
	void SetDockWidth(int DockWidth) {m_DockWidth = DockWidth;}

public:
	UINT m_DockState;
	int m_DockWidth;
};


#endif // DOCKABLE_H

