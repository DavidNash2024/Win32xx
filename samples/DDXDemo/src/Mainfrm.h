/////////////////////////////////////////
// Mainfrm.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "AboutDialog.h"

////////////////////////////////////////////////////////
// The CMainFrame class manages the frame window of the
// application.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

    CDoc&   TheDoc()   { return m_view.TheDoc(); }
    BOOL    UpdateDialog(BOOL bReadFromControl);

protected:
    virtual BOOL    LoadRegistrySettings(LPCWSTR keyName) override;
    virtual void    OnClose() override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL    OnHelp() override;
    virtual void    OnInitialUpdate() override;
    virtual void    OnMenuUpdate(UINT id) override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    BOOL    OnCheckButton(UINT id);
    BOOL    OnEditCopy();
    BOOL    OnEditCut();
    BOOL    OnEditDelete();
    BOOL    OnEditPaste();
    BOOL    OnEditRedo();
    BOOL    OnEditUndo();
    BOOL    OnFileExit();
    BOOL    OnRadioButton(UINT id);

    CAboutDialog m_aboutDialog;
    CView        m_view;
};

#endif //MAINFRM_H
