/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "AppHelp.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    // Virtual functions that override base class functions
    virtual BOOL    LoadRegistrySettings(LPCWSTR keyName) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnInitialUpdate() override;
    virtual void    OnMenuUpdate(UINT id) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual BOOL    SaveRegistrySettings() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    BOOL    ChooseHelpTopic();
    CString CreateAppDataFolder(const CString& subfolder);
    CDoc& GetDoc() { return m_view.GetDoc(); }
    UINT    GetIDFromCursorPos();
    BOOL    OnF1();
    BOOL    OnFileExit();
    BOOL    OnShiftF1();
    void    OnUpdateCheckA(UINT id);
    void    OnUpdateCheckB(UINT id);
    void    OnUpdateCheckC(UINT id);
    void    OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id);
    BOOL    ShowHelpTopic(UINT id);

    // Message handlers
    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CView       m_view;
    CHelp       m_appHelp;  // Help object for context help and help about dialog
    BOOL        m_isChoosing;
};


#endif //MAINFRM_H

