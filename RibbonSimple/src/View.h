/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

#include <wincore.h>
#include <ribbon.h>


class CView : public CWnd, public CRibbon
{
public:
	CView() : m_uRibbonHeight(0) {}
	virtual ~CView() {}
	virtual STDMETHODIMP Execute(UINT32 nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCommandExecutionProperties);
	virtual STDMETHODIMP OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode);

	UINT GetRibbonHeight() { return m_uRibbonHeight; }

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void OnPaint(HDC hDC);
	virtual void OnSize();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	UINT m_uRibbonHeight;

};

#endif
