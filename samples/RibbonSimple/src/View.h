/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H


//////////////////////////////////////////
// CView manages the main window. It uses a Ribbon
// in place of a menu and toolbar.
class CView : public CWnd, public CRibbon
{
public:
    CView() : m_pIUIRibbon(0) {}
    virtual ~CView() {}

protected:
    // Virtual functions that override base class functions
    virtual STDMETHODIMP Execute(UINT32, UI_EXECUTIONVERB, const PROPERTYKEY*, const PROPVARIANT*, IUISimplePropertySet*);
    virtual STDMETHODIMP OnViewChanged(UINT32, UI_VIEWTYPE, IUIApplication::IUnknown*, UI_VIEWVERB, INT32);

    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual LRESULT OnSize();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView(const CView&);                // Disable copy construction
    CView& operator = (const CView&);   // Disable assignment operator

    IUIRibbon* GetIUIRibbon() const;
    void RecalcLayout();

    // Member variables
    IUIRibbon* m_pIUIRibbon;
};


#endif
