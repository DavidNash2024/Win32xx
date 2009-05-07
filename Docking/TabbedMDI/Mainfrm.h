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
	enum DockIDs
	{
		ID_CLASSES1 = 1,
		ID_CLASSES2 = 2,
		ID_FILES1 = 3,
		ID_FILES2 = 4,
		ID_OUTPUT1 = 5,
		ID_OUTPUT2 = 6,
		ID_TEXT1 = 7,
		ID_TEXT2 = 8,
		ID_SIMPLE = 9,
		ID_RECT = 10
	};

	CMainFrame(void);
	virtual ~CMainFrame();
	void AddDocked(DockInfo di, CDocker* pDock);
	void AddUndocked(DockInfo di);
	void OnFileNew();
	void OnContainerTabsAtTop();
	void OnMDITabsAtTop();
	void LoadRegistryTabbedMDIs();
	void LoadDefaultDockers();
	void LoadDefaultMDITabs();
	void LoadDockers(tString tsRegistryKeyName);
	void SaveTabbedMDIs();
	
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void SaveRegistrySettings();
	virtual void SetupToolbar();

private:
	CDockTabbedMDI m_DockTabbedMDI;
};

#endif //MAINFRM_H

