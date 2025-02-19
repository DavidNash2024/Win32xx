/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "scintillaview.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();

    // Virtual functions that override public base class functions
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;
    CString GetFileName();
    void SetWindowTitle();
    void UpdateToolbar();

protected:
    // Virtual functions that override base class functions
    virtual void    OnClose() override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnInitialUpdate() override;
    virtual void    OnMenuUpdate(UINT id) override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetStatusIndicators() override;
    virtual void    SetStatusParts() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command Handlers
    BOOL    OnFileExit();
    BOOL    OnFileMRU(WPARAM wparam);
    BOOL    OnFileNew();
    BOOL    OnFileOpen();
    BOOL    OnFilePrint();
    BOOL    OnFileSave();
    BOOL    OnFileSaveAs();
    BOOL    OnFilePreview();
    BOOL    OnOptionsBackground();
    BOOL    OnOptionsFont();
    BOOL    OnOptionWordWrap();

    // Message handlers
    LRESULT OnDropFiles(UINT, WPARAM wparam, LPARAM);
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();

    int  AdjustForDPI(int value) const;
    int  GetTextPartWidth(LPCWSTR text) const;
    void RestoreFocus() { ::SetFocus(m_oldFocus); }
    void SaveFocus() { m_oldFocus = ::GetFocus(); }

    // Member variables
    ScintillaView m_view;
    CPrintPreview<ScintillaView> m_preview;
    CString m_pathName;
    CString m_cap;
    CString m_num;
    CString m_ovr;
    bool m_isToolbarShown;
    HWND m_oldFocus;
};

#endif //MAINFRM_H

