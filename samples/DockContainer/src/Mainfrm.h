/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "Classes.h"
#include "Files.h"
#include "Simple.h"
#include "Text.h"
#include "Output.h"


// Declaration of the CMainDockFrame class
class CMainDockFrame : public CFrame
{
public:
	CMainDockFrame();
	virtual ~CMainDockFrame();
	void LoadDefaultDockers();
	BOOL OnDockCloseAll();
	BOOL OnDockDefault();
	BOOL OnFileExit();
	
protected:
	virtual CDocker* NewDockerFromID(int nID);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual BOOL SaveRegistrySettings();
	virtual void SetupToolBar();

private:
	CViewSimple m_View;

};

#endif //MAINFRM_H

