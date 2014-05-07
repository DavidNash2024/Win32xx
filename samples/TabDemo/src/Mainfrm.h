/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "Classes.h"
#include "Files.h"
#include "MyDialog.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pCreateStruct);
	virtual void OnFileExit();
	virtual void OnInitialUpdate();
	virtual void OnMenuUpdate(UINT nID);
	virtual void OnNewClassesTab();
	virtual void OnNewDialogTab();
	virtual void OnNewFilesTab();
	virtual void OnNewTab();
	virtual void OnShowButtons();
	virtual void OnTabsAtTop();
	virtual void OnTabClose();
	virtual void OnTabFixedWidth();
	virtual void OnTabOwnerDraw();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CTab m_View;

};

#endif //MAINFRM_H

