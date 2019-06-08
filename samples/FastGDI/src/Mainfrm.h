/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    void ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL isGray);
    BOOL OnAdjustImage();
    BOOL OnFileExit();
    BOOL OnFileSave();
    BOOL OnFileSaveAs();
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileOpenMRU(WPARAM wparam, LPARAM lparam);
    BOOL OnFilePreview();
    BOOL OnFilePrint();

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

