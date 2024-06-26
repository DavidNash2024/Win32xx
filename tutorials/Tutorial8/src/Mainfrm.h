///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H

#include "targetver.h"
#include "wxx_commondlg.h"
#include "wxx_frame.h"
#include "wxx_file.h"
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
    CDoc& GetDoc() { return m_view.GetDoc(); }
    void LoadFile(LPCTSTR fileName);

    LRESULT OnDropFile(WPARAM wparam);

    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFilePrint();
    BOOL OnFileSave();
    BOOL OnFileSaveAs();
    BOOL OnPenColor();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);               // Disable copy construction
    CMainFrame& operator=(const CMainFrame&);    // Disable assignment operator

    CView m_view;
    CString m_pathName;
};

#endif //MAINFRM_H

