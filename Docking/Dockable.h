///////////////////////////////////////////////////////
// Dockable.h
//  Declaration of the CDockable class

#ifndef DOCKABLE_H
#define DOCKABLE_H


#include "../Win32++/WinCore.h"

enum Constants
{
	// Notification messages
	USER_DOCKDRAGSTART= WM_APP + 1, // mouse position during drag start
	USER_DOCKDRAGMOVE = WM_APP + 2,	// mouse position during drag move
	USER_DOCKDRAGEND  = WM_APP + 3, // mouse position during drag release
	USER_BARDRAGSTART = WM_APP + 4,
	USER_BARDRAGMOVE  = WM_APP + 5,
	USER_BARDRAGEND   = WM_APP + 6
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

