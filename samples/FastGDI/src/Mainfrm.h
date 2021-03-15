/////////////////////////////////////////////
// Mainfrm.h

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
    void ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL isGray);
    void OnAdjustImage();
    void OnFileExit();
    void OnFileSave();
    void OnFileSaveAs();
    void OnFileNew();
    void OnFileOpen();
    void OnFileOpenMRU(WPARAM wparam, LPARAM lparam);
    void OnFilePreview();
    void OnFilePrint();

    void OnPreviewClose();
    void OnPreviewPrint();
    void OnPreviewSetup();
    BOOL LoadFile(CString& fileName);
    void SaveFile(CString& fileName);

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT id);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CPrintPreview<CView> m_preview;   // CView is the source of the PrintPage function
    CString m_pathName;

};

#endif //MAINFRM_H

