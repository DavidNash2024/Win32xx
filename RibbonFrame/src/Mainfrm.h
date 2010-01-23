/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

#if defined __IUIRibbon_INTERFACE_DEFINED__
	HRESULT RibbonExecute(UINT32 nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCommandExecutionProperties);
#endif

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnFileOpen();
	virtual void OnFilePrint();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolbar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
};

#endif //MAINFRM_H

