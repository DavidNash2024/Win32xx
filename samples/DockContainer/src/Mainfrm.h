/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "Simple.h"


// Declaration of the CMainFrame class
class CMainFrame : public CDockFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();
	void LoadDefaultDockers();
	BOOL OnDockCloseAll();
	BOOL OnDockDefault();
	BOOL OnFileExit();
	
protected:
	virtual CDocker* NewDockerFromID(int nID);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual BOOL SaveRegistrySettings();
	virtual void SetupToolBar();

private:
	CViewSimple m_View;

};

#endif //MAINFRM_H

