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

    COLORREF GetColorFromPicker() const;
    CDoc& GetDoc() { return m_doc; }

    void OnFileExit();
    void OnFileNew();
    void OnMDIClose();
    void OnMDIMaximize();
    void OnMDIMaximized(BOOL isMax);
    void OnMDIMinimize();
    void OnMDIRestore();
    void OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
    void SetPenColor(COLORREF clr);

protected:
    virtual STDMETHODIMP Execute(UINT32 cmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
    virtual STDMETHODIMP UpdateProperty(UINT32 nCmdID, __in REFPROPERTYKEY key, __in_opt  const PROPVARIANT *currentValue, __out PROPVARIANT *newValue);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT &cs);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDoc m_doc;
    CString m_pathName;

};

#endif //MAINMDIFRM_H

