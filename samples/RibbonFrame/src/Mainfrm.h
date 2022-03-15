/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"


////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a statusbar and
// view window. It uses a ribbon in place of the menubar
// and toolbar.
class CMainFrame : public CRibbonFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

protected:
    // Virtual functions that override base class functions
    virtual STDMETHODIMP Execute(UINT32, UI_EXECUTIONVERB, const PROPERTYKEY*, const PROPVARIANT*, IUISimplePropertySet*);
    virtual STDMETHODIMP UpdateProperty(UINT32, __in REFPROPERTYKEY, __in_opt const PROPVARIANT*, __out PROPVARIANT*);
    virtual STDMETHODIMP OnViewChanged(UINT32, UI_VIEWTYPE typeId, IUnknown*, UI_VIEWVERB verb, INT32);

    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Command handlers
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
    void LoadFile(LPCTSTR str);
    void MRUFileOpen(UINT mruIndex);
    BOOL SetPenColor(COLORREF clr);

    // Member variables
    CView m_view;
    CString m_pathName;
    IUIRibbon* m_pIUIRibbon;
};


#endif //MAINFRM_H

