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
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

protected:
    // Virtual functions that override base class functions
    virtual BOOL    LoadRegistrySettings(LPCTSTR keyName);
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual void    OnMenuUpdate(UINT id);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual BOOL    SaveRegistrySettings();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);                // Disable copy construction
    CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

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

