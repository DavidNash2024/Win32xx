/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "Web.h"
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
	CMainFrame(void);
	virtual ~CMainFrame();
	void OnFileNew();
	void OnContainerTabsAtTop();
	void OnEditCopy();
	void OnEditPaste();
	void OnEditCut();
	void OnEditDelete();
	void OnEditRedo();
	void OnEditUndo();
	void OnMDITabsAtTop();
	void LoadDefaultDockers();
	void LoadDefaultMDIs();
	
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual BOOL SaveRegistrySettings();
	virtual void SetupToolBar();

private:
	CDockTabbedMDI m_DockTabbedMDI;
};

#endif //MAINFRM_H

