/////////////////////////////////////////
// Mainfrm.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H

#include "View.h"
#include "AboutBox.h"

//////////////////////////////////////////////////////////////////////////////
// The main application frame window. It contains the menu, toolbar, status
// bar, and the view. It is also responsible for handling the command messages
// from the menu and toolbar, and for initializing the view and the about
// dialog.
class CMainFrame : public CFrame
{
public:
                CMainFrame();
    virtual     ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;
    void        SetAppName(const CString& appName)
                                { m_appName = appName; }
    void        SetWindowTitle(const CString &title = L"");
    CView&      TheView()       {return m_view;}
    CDoc&       ThisDoc()       {return m_view.TheDoc();}
    AboutBox&   GetAboutBox()   { return m_aboutDialog; }

    static const CString m_compiledOn; // Compilation date.

protected:

    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnInitialUpdate() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    void    OnColorChoice();
    void    OnFileExit();
    void    OnFileNew();
    void    OnFileOpen();
    void    OnFileSave();
    void    OnFileSaveAs();
    void    OnFontChoice();
    BOOL    OnHelpAbout();

    // Member variables.
    AboutBox   m_aboutDialog;  // About dialog object
    CView      m_view;         // The view object
    CString    m_appName;      // Application name
};

#endif // SDI_MAINFRM_H


