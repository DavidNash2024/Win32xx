///////////////////////////////////////////////////////
// DockContainer.h
//  Declaration of the CDockContainer class

#ifndef DOCKCONTAINER_H
#define DOCKCONTAINER_H


#include "../Win32++/WinCore.h"

class CDockContainer : public CWnd
{
public:
	CDockContainer();
	virtual ~CDockContainer() {}
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL IsDocked() {return m_IsDocked;}

private:
	BOOL m_IsDocked;
};


#endif // DOCKCONTAINER_H

