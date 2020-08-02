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
    void OnFilePrint();
    void OnFileSave();
    void OnFileSaveAs();
    void OnPenColor();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CString m_pathName;

};

#endif //MAINFRM_H

