/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

#include <wincore.h>
#include <ribbon.h>


class CView : public CWnd
{
public:
	CView() {}
	virtual ~CView() {}
	void DestroyRibbon();
	bool CreateRibbon();

	virtual HRESULT OnRibbonExecute(UINT nCmdID, UI_EXECUTIONVERB verb, __in_opt const PROPERTYKEY* key, __in_opt const PROPVARIANT* ppropvarValue, 
											  __in_opt IUISimplePropertySet* pCommandExecutionProperties);

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CRibbonApplication* m_pRibbonApp;
	IUIFramework* m_pRibbonFramework;
};

#endif
