/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CMainFrame class declaration. This class
    controls the appearance and frame-related actions of the DDXDemo program.
    This class presents a fixed-size frame whose status bar has no resizing
    gripper in the lower-right-hand corner.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"

/*============================================================================*/
    class
CMainFrame : public CFrame                                                  /*

    Declaration of the CMainFrame class of the App-Frame-Doc-View architecture.
*----------------------------------------------------------------------------*/
{
    public:
        CMainFrame(void);
        virtual ~CMainFrame() {}

        CDoc&   TheDoc()   { return m_view.TheDoc(); }
        CView&  TheView() { return m_view; };

        BOOL    UpdateDialog(BOOL bReadFromControl = SENDTOCONTROL);

    private:
        BOOL    LoadRegistrySettings(LPCTSTR keyName);
        void    OnClose();
        BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
        int     OnCreate(CREATESTRUCT& cs);
        void    OnFileExit();
        void    OnInitialUpdate();
        void    PreCreate(CREATESTRUCT& cs);
        void    SetReBarColors(COLORREF, COLORREF, COLORREF, COLORREF);
        BOOL    SetThemeColors();
        void    SetupToolBar();
        LRESULT WndProc(UINT msg, WPARAM, LPARAM);

        CView             m_view;
        std::vector<UINT> m_bandIDs;
        std::vector<UINT> m_bandStyles;
        std::vector<UINT> m_bandSizes;
};
/*----------------------------------------------------------------------------*/
#endif //MAINFRM_H

