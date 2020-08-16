/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"

///////////////////////////////////////
// Declaration of the CMainFrame class.
//
class CMainFrame : public CRibbonFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    CDoc& GetDoc() { return m_view.GetDoc(); }
    void LoadFile(LPCTSTR str);
    void MRUFileOpen(UINT mruIndex);

    LRESULT OnDropFile(WPARAM wparam);
    void OnFileExit();
    void OnFileNew();
    void OnFileOpen();
    void OnFileSave();
    void OnFileSaveAs();
    void OnFilePrint();
    void OnMRUList(const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue);
    void OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
    void SetPenColor(COLORREF clr);

protected:
    virtual STDMETHODIMP Execute(UINT32 cmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
    virtual STDMETHODIMP UpdateProperty(UINT32 cmdID, __in REFPROPERTYKEY key, __in_opt  const PROPVARIANT *currentValue, __out PROPVARIANT *newValue);
    BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CString m_pathName;
};

#endif //MAINFRM_H

