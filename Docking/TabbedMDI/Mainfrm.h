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

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	void OnFileNew();
	void OnContainerTabsAtTop();
	void OnMDITabsAtTop();
	void LoadDefaultDockers();
	void LoadDefaultMDIs();
	
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

