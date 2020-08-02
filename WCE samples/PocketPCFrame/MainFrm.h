#ifndef MAINFRM_H
#define MAINFRM_H


#include "wxx_wincore.h"
#include "wxx_wceframe.h"
#include "wxx_dialog.h"


class CMainFrame : public CWceFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif   // MAINFRM_H
