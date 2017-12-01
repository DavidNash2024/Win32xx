///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "SplitterPanes.h"


class CMainFrame : public CDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    
    BOOL OnFileExit();
    BOOL OnViewList();
    BOOL OnViewText();

protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual void OnInitialUpdate();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CViewSimple m_MainView;
    CDockText* m_pDockText;
    CDockTree* m_pDockTree;
    CDockList* m_pDockList;
};

#endif //MAINFRM_H

