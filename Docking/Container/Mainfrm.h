/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../../Win32++/frame.h"
#include "Classes.h"
#include "Files.h"
#include "Simple.h"
#include "Text.h"
#include "Output.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	struct DockedInfo
	{
		DWORD DockStyle;
		int DockWidth;
		int DockID;
		int DockParentID;
		RECT Rect;
	};

	enum DockIDs
	{
		ID_CONTAINCLASSES1 = 1,
		ID_CONTAINFILES1 = 2,
		ID_CONTAINCLASSES2 = 3,
		ID_CONTAINFILES2 = 4,
		ID_TEXT = 5,
		ID_OUTPUT = 6
	};

	CMainFrame(void);
	virtual ~CMainFrame();
	void LoadDefaultDockables();
	void LoadRegistryDockables();
	void SaveDockables();
	void SaveRegistrySettings();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CDockSimple m_DockView;

};

#endif //MAINFRM_H

