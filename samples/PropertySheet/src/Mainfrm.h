/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"
#include "MyPropertySheet.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    BOOL OnFileExit();  
    BOOL OnModal();
    BOOL OnModeless();
    BOOL OnWizard();

protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CView m_View;
    CMyPropertySheet m_ModelessPS;
};

#endif //MAINFRM_H

