///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H

#include "wxx_commondlg.h"
#include "wxx_frame.h"
#include "wxx_file.h"
#include "wxx_preview.h"
#include "View.h"
#include "Doc.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override;
    CDoc& GetDoc() { return m_view.GetDoc(); }
    void LoadFile(LPCWSTR fileName);

protected:
    virtual void OnClose() override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnInitialUpdate() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileSave();
    BOOL OnFileSaveAs();
    BOOL OnFilePreview();
    BOOL OnFilePrint();
    BOOL OnPenColor();
    LRESULT OnDropFile(WPARAM wparam);
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();

    CView m_view;
    CPrintPreview<CView> m_preview;   // CView is the source of the PrintPage function.
    CString m_pathName;
    BOOL m_isToolbarShown;
};

#endif // MAINFRM_H

