///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"


class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    CDoc& GetDoc() { return m_view.GetDoc(); }
    void LoadFile(LPCTSTR fileName);

    void OnPreviewClose();
    LRESULT OnDropFile(WPARAM wparam);
    void OnFileExit();
    void OnFileMRU(WPARAM wparam);
    void OnFileNew();
    void OnFileOpen();
    void OnFileSave();
    void OnFileSaveAs();
    void OnFilePreview();
    void OnFilePrint();
    void OnPenColor();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CPrintPreview<CView> m_preview;
    CString m_pathName;

};

#endif //MAINFRM_H

