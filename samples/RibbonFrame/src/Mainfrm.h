/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"


////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a statusbar and
// view window. It uses a ribbon in place of the menubar
// and toolbar.
class CMainFrame : public CRibbonFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    // Virtual functions that override base class functions.
    virtual STDMETHODIMP Execute(UINT32, UI_EXECUTIONVERB, const PROPERTYKEY*, const PROPVARIANT*, IUISimplePropertySet*) override;
    virtual STDMETHODIMP UpdateProperty(UINT32, __in REFPROPERTYKEY, __in_opt const PROPVARIANT*, __out PROPVARIANT*) override;
    virtual STDMETHODIMP OnViewChanged(UINT32, UI_VIEWTYPE typeId, IUnknown*, UI_VIEWVERB verb, INT32) override;

    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers.
    LRESULT OnDropFile(WPARAM wparam);
    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileSave();
    BOOL OnFileSaveAs();
    BOOL OnFilePrint();
    BOOL OnMRUList(const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue);
    BOOL OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);

    IUIRibbon* GetIUIRibbon() const;
    CDoc& GetDoc() { return m_view.GetDoc(); }
    void LoadFile(LPCWSTR str);
    void MRUFileOpen(UINT mruIndex);
    BOOL SetPenColor(COLORREF clr);

    // Member variables.
    CView m_view;
    CString m_pathName;
    IUIRibbon* m_pIUIRibbon;
};


#endif // MAINFRM_H

