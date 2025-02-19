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
    CView() : m_pIUIRibbon(nullptr) {}
    virtual ~CView() override = default;

protected:
    // Virtual functions that override base class functions
    virtual STDMETHODIMP Execute(UINT32, UI_EXECUTIONVERB, const PROPERTYKEY*, const PROPVARIANT*, IUISimplePropertySet*) override;
    virtual STDMETHODIMP OnViewChanged(UINT32, UI_VIEWTYPE, IUIApplication::IUnknown*, UI_VIEWVERB, INT32) override;

    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnDestroy() override;
    virtual void OnDraw(CDC& dc) override;
    virtual void OnInitialUpdate() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    IUIRibbon* GetIUIRibbon() const;
    LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSize();
    void RecalcLayout();

    // Member variables
    IUIRibbon* m_pIUIRibbon;
};


#endif
