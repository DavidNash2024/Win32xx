/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

#include <wincore.h>
#include <ribbon.h>


class CView : public CWnd, public CRibbon
{
public:
	CView() {}
	virtual ~CView() {}
	virtual STDMETHODIMP Execute(UINT32 nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCommandExecutionProperties);
	virtual STDMETHODIMP OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode);

protected:
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnDestroy();
	virtual void OnDraw(CDC& dc);	
	virtual void OnInitialUpdate();
	virtual void OnSize();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif
