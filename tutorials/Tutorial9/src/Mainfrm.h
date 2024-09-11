///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H

#include "wxx_commondlg.h"
#include "wxx_frame.h"
#include "wxx_file.h"
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

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    void LoadFile(LPCWSTR fileName);
    LRESULT OnDropFile(WPARAM wparam);
    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileSave();
    BOOL OnFileSaveAs();
    BOOL OnFilePrint();
    BOOL OnPenColor();

    CView m_view;
    CString m_pathName;
};

#endif // MAINFRM_H

