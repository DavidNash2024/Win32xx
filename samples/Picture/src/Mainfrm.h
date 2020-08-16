/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"

//////////////////////////////////////
// Declaration of the CMainFrame class
//
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    BOOL OnFileExit();
    BOOL OnFileMRU(WPARAM wparam);
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileSaveAs();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual LRESULT OnFileLoaded(LPCTSTR fileName);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
};

#endif //MAINFRM_H

