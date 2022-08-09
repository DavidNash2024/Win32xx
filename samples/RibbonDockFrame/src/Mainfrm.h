/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "Doc.h"


////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a statusbar and
// view window. It uses a ribbon in place of the menubar
// and toolbar. The frame window supports docking.
class CMainFrame : public CRibbonDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

protected:
    // Virtual functions that override base class functions
    virtual STDMETHODIMP Execute(UINT32, UI_EXECUTIONVERB, const PROPERTYKEY*, const PROPVARIANT*, IUISimplePropertySet*);
    virtual STDMETHODIMP OnViewChanged(UINT32, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT32);
    virtual STDMETHODIMP UpdateProperty(UINT32, __in REFPROPERTYKEY, __in_opt  const PROPVARIANT*, __out PROPVARIANT*);

    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnInitialUpdate();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);                // Disable copy construction
    CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

    // Command handlers
    void OnFileExit();
    void OnFileNew();
    void OnFileOpen();
    void OnFileSave();
    void OnFileSaveAs();
    void OnFilePrint();
    void OnMRUList(const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue);
    void OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);

    // Message handlers
    LRESULT OnDropFile(WPARAM wparam);
    LRESULT OnGetAllPoints();
    LRESULT OnSendPoint(WPARAM wparam);

    IUIRibbon* GetIUIRibbon() const;
    CDoc& GetDoc() { return m_doc; }
    void LoadFile(LPCTSTR fileName);
    void MRUFileOpen(UINT mruIndex);
    void SetPenColor(COLORREF clr);

    // Member variables
    CView m_view;
    CDoc m_doc;
    CString m_pathName;
    IUIRibbon* m_pIUIRibbon;
};

#endif //MAINFRM_H
