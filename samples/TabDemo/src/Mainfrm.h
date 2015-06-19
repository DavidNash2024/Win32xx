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
	CMainFrame();
	virtual ~CMainFrame();

	BOOL OnFileExit();
	BOOL OnNewClassesTab();
	BOOL OnNewDialogTab();
	BOOL OnNewFilesTab();
	BOOL OnNewTab();
	BOOL OnShowButtons();
	BOOL OnTabsAtTop();
	BOOL OnTabClose();
	BOOL OnTabFixedWidth();
	BOOL OnTabOwnerDraw();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pCreateStruct);
	virtual void OnInitialUpdate();
	virtual void OnMenuUpdate(UINT nID);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CTab m_View;

};

#endif //MAINFRM_H

