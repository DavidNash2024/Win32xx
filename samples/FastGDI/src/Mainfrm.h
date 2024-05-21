/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = NULL);

    void DpiScaleToolBar();
    BOOL LoadFile(CString& fileName);
    void SaveFile(CString& fileName);

protected:
    // Virtual functions that override base class functions
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT id);
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);               // Disable copy construction
    CMainFrame& operator=(const CMainFrame&);    // Disable assignment operator

    // Command handlers
    BOOL OnAdjustImage();
    BOOL OnFileExit();
    BOOL OnFileSave();
    BOOL OnFileSaveAs();
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileOpenMRU(WPARAM wparam, LPARAM lparam);
    BOOL OnFilePreview();
    BOOL OnFilePrint();

    // Message handlers
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();
    LRESULT OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam);

    void ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL isGray);

    // Member variables
    CView m_view;
    CPrintPreview<CView> m_preview;   // CView is the source of the PrintPage function
    CString m_pathName;
    CRect m_viewRect;
    CPoint m_scrollPos;
    bool m_isDPIChanging;
    bool m_isToolbarShown;
};

#endif //MAINFRM_H

