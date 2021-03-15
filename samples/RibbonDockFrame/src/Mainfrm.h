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
    CDoc& GetDoc() { return m_doc; }
    void LoadFile(LPCTSTR fileName);
    void MRUFileOpen(UINT mruIndex);

    LRESULT OnDropFile(WPARAM wparam);
    LRESULT OnGetAllPoints();
    LRESULT OnSendPoint(WPARAM wparam);

    void OnFileExit();
    void OnFileNew();
    void OnFileOpen();
    void OnFileSave();
    void OnFileSaveAs();
    void OnFilePrint();
    void OnInitialUpdate();
    void OnMRUList(const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue);
    void OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
    void SetPenColor(COLORREF clr);

protected:
    virtual STDMETHODIMP Execute(UINT32 cmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
    virtual STDMETHODIMP UpdateProperty(UINT32 cmdID, __in REFPROPERTYKEY key, __in_opt  const PROPVARIANT *currentValue, __out PROPVARIANT *newValue);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CDoc m_doc;
    CString m_pathName;
};

#endif //MAINFRM_H
