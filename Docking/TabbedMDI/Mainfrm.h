/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../../Win32++/frame.h"
#include "Simple.h"
#include "Rect.h"
#include "Classes.h"
#include "Files.h"
#include "DockTabbedMDI.h"
#include "Text.h"
#include "Output.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	struct DockInfo
	{
		DWORD DockStyle;
		int DockWidth;
		int DockID;
		int DockParentID;
		RECT Rect;
	};

	enum DockIDs
	{
		ID_CONTAINCLASSES1 = 1,
		ID_CONTAINFILES1 = 2,
		ID_CONTAINCLASSES2 = 3,
		ID_CONTAINFILES2 = 4,
		ID_TEXT1 = 5,
		ID_OUTPUT1 = 6,
		ID_TEXT2 = 7,
		ID_OUTPUT2 = 8
	};

	CMainFrame(void);
	virtual ~CMainFrame();
	void AddDocked(DockInfo di, CDockable* pDock);
	void AddUndocked(DockInfo di);
	void DoPopupMenu();
	void LoadDefaultDockables();
	void LoadDefaultMDITabs();
	void SaveDockables();
	
protected:
	virtual void LoadRegistryDockables();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void SaveRegistrySettings();
	virtual void SetupToolbar();

private:
	CDockTabbedMDI m_DockView;

};

#endif //MAINFRM_H

