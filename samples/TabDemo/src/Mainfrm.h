/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


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
    BOOL OnFileExit();
    BOOL OnNewClassesTab();
    BOOL OnNewDialogTab();
    BOOL OnNewFilesTab();
    BOOL OnNewTab();
    BOOL OnShowButtons();
    BOOL OnTabsAtTop();
    BOOL OnTabClose();
    BOOL OnTabFixedWidth();
    BOOL OnTabOwnerDraw();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT id);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();

private:
    CTab m_view;
};

#endif //MAINFRM_H
