/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "Dockers.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	void LoadDefaultDockers();
	void SetDockStyles();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnDockCloseAll();
	virtual void OnDockDefault();
	virtual void OnFileExit();
	virtual void OnInitialUpdate();
	virtual void OnPropResize();
	virtual void On3DBorder();
	virtual void OnUndockable();
	virtual void OnMenuUpdate(UINT nID);
	virtual void OnNoResize();
	virtual void OnNoDockLR();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual BOOL SaveRegistrySettings();
	virtual void SetupToolBar();

private:
	CDockSimple m_DockView;
	BOOL m_bProportionalResize;
	BOOL m_b3DBorder;
	BOOL m_bUndockable;
	BOOL m_bUnMoveable;
	BOOL m_bNoDockLR;
};

#endif //MAINFRM_H

