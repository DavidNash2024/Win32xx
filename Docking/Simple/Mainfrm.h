/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../../Win32++/frame.h"
#include "Dockers.h"


enum DockIDs
{
	ID_CLASSES1 = 1,
	ID_CLASSES2 = 2,
	ID_FILES1 = 3,
	ID_FILES2 = 4,
	ID_SIMPLE1 = 5,
	ID_SIMPLE2 = 6,
	ID_TEXT1 = 7,
	ID_TEXT2 = 8
};

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

