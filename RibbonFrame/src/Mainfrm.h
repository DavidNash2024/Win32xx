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
	virtual void MRUFileOpen(UINT nMRUIndex);
	virtual STDMETHODIMP Execute(UINT32 nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCommandExecutionProperties);
	virtual STDMETHODIMP UpdateProperty(UINT32 nCmdID, __in REFPROPERTYKEY key, __in_opt  const PROPVARIANT *currentValue, __out PROPVARIANT *newValue);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnFileNew();
	virtual void OnFileOpen();
	virtual void OnFileSave();
	virtual void OnFileSaveAs();
	virtual void OnFilePrint();
	virtual void OnInitialUpdate();	
	virtual void SetupToolbar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	tString m_PathName;
};

#endif //MAINFRM_H

