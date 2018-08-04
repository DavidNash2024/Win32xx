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

    BOOL OnFileExit();
    BOOL OnFileOpen();
    BOOL OnFilePrint();
    BOOL OnFileSave();
    void OnBottom();
    void OnLeft();
    void OnRight();
    void OnTop();
    void SetReBarPos();
    void SetWrapState(CToolBar* pToolBar, BOOL isWrapped);

protected:
    virtual CRect   GetViewRect() const;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void    SetupToolBar();
    virtual void    RecalcLayout();
    virtual LRESULT WndProc(UINT msg, WPARAM wmaram, LPARAM lmaram);

private:
    CReBar m_reBar;
    CToolBar m_toolBar;
    CImageList m_toolBarImages;
    CView m_view;
};

#endif //MAINFRM_H

