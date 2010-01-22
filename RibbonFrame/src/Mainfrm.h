/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include <ribbon.h>
#include "View.h"

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	HRESULT RibbonExecute(UINT nCmdID, UINT verb, LPCVOID key, LPCVOID ppropvarValue, LPVOID pCommandExecutionProperties);
	HRESULT RibbonOnViewChanged(UINT viewId, UINT typeId, void* pView, UINT verb, INT uReasonCode);

protected:
	virtual CRect GetViewRect() const;
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
	UINT m_uRibbonHeight;
	CRibbon m_Ribbon;
};

#endif //MAINFRM_H

