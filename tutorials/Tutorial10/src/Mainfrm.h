///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H

#include "targetver.h"
#include "wxx_commondlg.h"
#include "wxx_frame.h"
#include "wxx_file.h"
#include "wxx_preview.h"
#include "View.h"
#include "Doc.h"


class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    CDoc& GetDoc() { return m_view.GetDoc(); }
    void LoadFile(LPCTSTR fileName);

    LRESULT OnDropFile(WPARAM wparam);

    void OnFileExit();
    void OnFileNew();
    void OnFileOpen();
    void OnFileSave();
    void OnFileSaveAs();
    void OnFilePreview();
    void OnFilePrint();
    void OnPenColor();
    void OnPreviewClose();
    void OnPreviewPrint();
    void OnPreviewSetup();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);    
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CPrintPreview<CView> m_preview;   // CView is the source of the PrintPage function
    CString m_pathName;

};

#endif //MAINFRM_H

