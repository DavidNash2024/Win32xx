///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"

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
    CDoc& GetDoc() { return m_view.GetDoc(); }
    void LoadFile(LPCTSTR fileName);

    LRESULT OnDropFile(WPARAM wparam);
    BOOL OnFileExit();
    BOOL OnFileMRU(WPARAM wparam);
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileSave();
    BOOL OnFileSaveAs();
    BOOL OnFilePreview();
    BOOL OnFilePrint();
    BOOL OnPenColor();
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CPrintPreview<CView> m_preview;   // CView is the source of the PrintPage function
    CString m_pathName;
};

#endif //MAINFRM_H

