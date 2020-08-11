/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


class CView : public CWnd, public CRibbon
{
public:
    CView() {}
    virtual ~CView() {}
    virtual STDMETHODIMP Execute(UINT32 cmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCommandExecutionProperties);
    virtual STDMETHODIMP OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb, INT32 reasonCode);

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual void OnSize();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

};

#endif
