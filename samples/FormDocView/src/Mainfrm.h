/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "FormView.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    CFormDoc& GetDoc()      { return m_formView.GetDoc(); }

    // Virtual functions that override public base class functions
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    // Virtual functions that override base class functions
    virtual BOOL LoadRegistrySettings(LPCWSTR keyName) override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL OnFileExit();
    virtual void OnInitialUpdate() override;
    virtual void OnMenuUpdate(UINT nID) override;
    virtual BOOL SaveRegistrySettings() override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Menu handlers
    void OnUpdateCheckA(UINT id);
    void OnUpdateCheckB(UINT id);
    void OnUpdateCheckC(UINT id);
    void OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id);

    // Member variables
    CFormView m_formView;
};


#endif //MAINFRM_H
