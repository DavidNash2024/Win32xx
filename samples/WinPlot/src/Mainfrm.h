/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "InputDlg.h"

///////////////////////////////////////
// Declaration of the CMainFrame class.
//
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    BOOL    OnEditFunction();
    BOOL    OnFileExit();

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
};

#endif //MAINFRM_H
