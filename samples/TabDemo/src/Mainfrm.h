/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


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
	virtual int  OnCreate(LPCREATESTRUCT pCreateStruct);
	virtual void OnFileClose();
	virtual void OnFileExit();
	virtual void OnFileNew();
	virtual void OnInitialUpdate();
	virtual void OnShowButtons();
	virtual void OnTabsAtTop();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CTab m_View;

};

#endif //MAINFRM_H

