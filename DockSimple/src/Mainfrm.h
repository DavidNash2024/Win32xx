/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../../Win32++/frame.h"
#include "Dockers.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	void LoadDefaultDockers();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual void SaveRegistrySettings();
	virtual void SetupToolbar();

private:
	CDockSimple m_DockView;
};

#endif //MAINFRM_H

