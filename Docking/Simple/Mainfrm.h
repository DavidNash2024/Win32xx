/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../../Win32++/frame.h"
#include "Dockables.h"


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
		ID_CLASS1 = 1,
		ID_CLASS2 = 2,
		ID_FILES1 = 3,
		ID_FILES2 = 4,
		ID_SIMPLE1 = 5,
		ID_SIMPLE2 = 6,
		ID_TEXT1 = 7,
		ID_TEXT2 = 8
	};

	CMainFrame(void);
	virtual ~CMainFrame();
	void AddDocked(DockInfo di, CDockable* pDock);
	void AddUndocked(DockInfo di);
	void LoadDefaultDockables();
	void LoadRegistryDockables();
	void SaveDockables();
	void SaveRegistrySettings();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	CDockSimple m_DockView;
};

#endif //MAINFRM_H

