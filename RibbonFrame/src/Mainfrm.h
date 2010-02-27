/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"
#include "Ribbon.h"


// Declaration of the CMainFrame class
class CMainFrame : public CRibbonFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	virtual STDMETHODIMP Execute(UINT32 nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCommandExecutionProperties);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnFileOpen();
	virtual void OnFilePrint();
	virtual void OnFileSave();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolbar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
};

#endif //MAINFRM_H

