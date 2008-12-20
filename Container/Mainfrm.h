/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../Win32++/frame.h"
#include "../Win32++/dockable.h"
#include "Classes.h"
#include "Files.h"


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
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CContainClasses m_Classes;
	CContainFiles m_Files;

};

#endif //MAINFRM_H

