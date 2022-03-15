/////////////////////////////
// MainMDIfrm.h
//

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H

#include "Doc.h"


///////////////////////////////////////////////////////////
// CMainMDIFrame manages the application's main window.
// The main window is a MDI frame which has a statusbar.
// It uses a ribbon in place of the toolbar and menubar.
class CMainMDIFrame : public CRibbonMDIFrame
{
public:
    CMainMDIFrame();
    virtual ~CMainMDIFrame();
    virtual HWND Create(HWND parent = 0);
    COLORREF GetColorFromPicker() const;
    CDoc& GetDoc() { return m_doc; }
    IUIRibbon* GetIUIRibbon() const;
    void SetPenColor(COLORREF clr);

protected:
    // Virtual functions that override base class functions
    virtual STDMETHODIMP Execute(UINT32, UI_EXECUTIONVERB, const PROPERTYKEY*, const PROPVARIANT*, IUISimplePropertySet*);
    virtual STDMETHODIMP UpdateProperty(UINT32, __in REFPROPERTYKEY, __in_opt const PROPVARIANT*, __out PROPVARIANT*);
    virtual STDMETHODIMP OnViewChanged(UINT32, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT32);

    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT &cs);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Command handlers
    void OnFileExit();
    void OnFileNew();
    void OnMDIClose();
    void OnMDIMaximize();
    void OnMDIMaximized(BOOL isMax);
    void OnMDIMinimize();
    void OnMDIRestore();
    void OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);

    // Member variables
    CDoc m_doc;
    CString m_pathName;
    IUIRibbon* m_pIUIRibbon;
};


#endif //MAINMDIFRM_H

