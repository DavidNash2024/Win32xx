/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "SplitterPanes.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    BOOL OnFileExit();
    BOOL OnViewList();
    BOOL OnViewText();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnInitialUpdate();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewSimple m_mainView;
    CDockText* m_pDockText;
    CDockTree* m_pDockTree;
    CDockList* m_pDockList;
};


#endif //MAINFRM_H

