/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "Dockers.h"

enum DockIDs
{
	ID_DOCK_CLASSES1 = 1,
	ID_DOCK_CLASSES2 = 2,
	ID_DOCK_FILES1 = 3,
	ID_DOCK_FILES2 = 4,
	ID_DOCK_SIMPLE1 = 5,
	ID_DOCK_SIMPLE2 = 6,
	ID_DOCK_TEXT1 = 7,
	ID_DOCK_TEXT2 = 8
};

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	void LoadDefaultDockers();
	void SetDockStyles();

protected:
	virtual CDocker* NewDockerFromID(int nID);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnDockCloseAll();
	virtual void OnDockDefault();
	virtual void OnDynamicResize();
	virtual void OnFileExit();
	virtual void OnInitialUpdate();
	virtual void OnPropResize();
	virtual void On3DBorder();
	virtual void OnNoUndocking();
	virtual void OnMenuUpdate(UINT nID);
	virtual void OnNoResize();
	virtual void OnNoDockLR();
	virtual void OnNoDockClose();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual BOOL SaveRegistrySettings();
	virtual void SetupToolBar();

private:
	CViewSimple m_View;
	BOOL m_UseProportionalResize;
	BOOL m_Use3DBorder;
	BOOL m_DisableUndocking;
	BOOL m_DisableResize;
	BOOL m_DisableDockLR;
	BOOL m_DisableDockClose;
	BOOL m_UseDynamicResize;
};

#endif //MAINFRM_H

