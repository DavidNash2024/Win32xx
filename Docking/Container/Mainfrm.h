/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../../Win32++/frame.h"
#include "Classes.h"
#include "Files.h"
#include "Simple.h"
#include "Text.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:

	enum DockIDs
	{
		ID_CLASS1 = 1,
		ID_CLASS2 = 2,
		ID_FILES1 = 3,
		ID_FILES2 = 4,
		ID_SIMPLE1 = 5,
		ID_SIMPLE2 = 6,
		ID_TEXT1 = 7,
		ID_TEXT2 = 8,
		ID_CONTAINCLASSES = 9,
		ID_CONTAINFILES = 10
	};

	CMainFrame(void);
	virtual ~CMainFrame();

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

