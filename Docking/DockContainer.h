///////////////////////////////////////////////////////
// DockContainer.h
//  Declaration of the CDockContainer class

#ifndef DOCKCONTAINER_H
#define DOCKCONTAINER_H


#include "../Win32++/WinCore.h"

enum Constants
{
	// Notification messages
	USER_DRAGSTART= WM_APP + 1, // mouse position during drag start
	USER_DRAGMOVE = WM_APP + 2,	// mouse position during drag move
	USER_DRAGEND  = WM_APP + 3  // mouse position during drag release
};

typedef struct DRAGPOS 
{
    NMHDR hdr;
    POINT ptPos;
} *LPDRAGPOS;

class CDockContainer : public CWnd
{
public:
	CDockContainer();
	virtual ~CDockContainer() {}
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL IsDocked() {return (BOOL)m_DockState;}
	UINT GetDockState() {return m_DockState;}
	void SetDockState(UINT uDockState) {m_DockState = uDockState;}
	virtual void SendNotify(UINT nMessageID);
	virtual void UnDock();

private:
	UINT m_DockState;
};


#endif // DOCKCONTAINER_H

