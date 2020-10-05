/* [Tab/Indent: 8/8][Line/Box: 80/74]                             (MainFrm.h) *
********************************************************************************

    Contents Description: Declaration of the CMainFrame class for a sample
    program demonstrating the use of a custom status bar within the Win32++
    Windows framework.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "CustomStatusBar.h"

/*============================================================================*/
    class
CMainFrame : public CFrame                                                  /*

    Declaration of the CMainFrame class.
*-----------------------------------------------------------------------------*/
{
    public:
        CMainFrame();
        virtual ~CMainFrame() {}

        BOOL    OnFileExit();

    protected:
        virtual CStatusBar& GetStatusBar() const { return m_customStatusBar; }
        virtual BOOL    OnCommand(WPARAM, LPARAM);
        virtual int     OnCreate(CREATESTRUCT& cs);
        virtual void    OnInitialUpdate();
        virtual void    PreCreate(CREATESTRUCT&);
        virtual void    SetStatusIndicators();
        virtual void    SetupToolBar();

    private:
        CView   m_view;
        mutable CustomStatusBar m_customStatusBar;
};
/*----------------------------------------------------------------------------*/
#endif //MAINFRM_H

