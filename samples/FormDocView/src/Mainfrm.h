/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "FormView.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);
    CFormDoc& GetDoc()      { return m_sdiView.GetDoc(); }

protected:
    virtual BOOL LoadRegistrySettings(LPCTSTR keyName);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnFileExit();
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT nID);
    virtual BOOL SaveRegistrySettings();
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void OnUpdateCheckA(UINT id);
    void OnUpdateCheckB(UINT id);
    void OnUpdateCheckC(UINT id);
    void OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id);

    CFormView m_sdiView;
};


#endif //MAINFRM_H
