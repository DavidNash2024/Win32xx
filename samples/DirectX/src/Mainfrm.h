/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "ViewThread.h"
#include "View.h"

// User defined window message
#define UWM_VIEWCREATED		(WM_APP + 1)	// Message - posted when view window is created

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	BOOL OnFileExit();
	BOOL OnFileOpen();
	BOOL OnFilePrint();
	BOOL OnFileSave();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CViewThread m_ViewThread;
};

#endif //MAINFRM_H

