/* (21-Oct-2024) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2020                |
|                                                                              |
===============================================================================*

    Contents Description:  Declaration of the CMainFrm class for this
    application using the Win32++ framework, Copyright (c) 2005-2020 David Nash,
    under permissions granted therein.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H

/******************************************************************************/

#include "View.h"
#include "AboutBox.h"


/*============================================================================*/
    class
CMainFrame : public CFrame                      /*

    The application mainframe class that creates the main menu, toolbar,
    status bar, and CView display, and responds to user interactions.
*-----------------------------------------------------------------------------*/
{
    public:
                    CMainFrame();
        virtual     ~CMainFrame() override = default;
        virtual HWND Create(HWND parent = nullptr) override;
        void        SetAppName(const CString& appName)
                                    { m_appName = appName; }
        void        SetWindowTitle(const CString &title = L"");
        CView&      TheView()       {return m_view;}
        CDoc&       ThisDoc()       {return m_view.TheDoc();}
        AboutBox&   GetAboutBox()   { return m_aboutDialog; }

        static const CString m_compiledOn; // compilation date

    protected:

        virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
        virtual int     OnCreate(CREATESTRUCT& cs) override;
        virtual void    OnInitialUpdate() override;
        virtual void    SetupMenuIcons() override;
        virtual void    SetupToolBar() override;
        virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

    private:
        CMainFrame(const CMainFrame&) = delete;
        CMainFrame& operator=(const CMainFrame&) = delete;

        void    OnColorChoice();
        void    OnFileExit();
        void    OnFileNew();
        void    OnFileOpen();
        void    OnFileSave();
        void    OnFileSaveAs();
        void    OnFontChoice();
        BOOL    OnHelpAbout();

          // private data members
        AboutBox   m_aboutDialog;  // about dialog object
        CView      m_view;         // the view object

        CString    m_appName;      // application name
};
/*----------------------------------------------------------------------------*/
#endif // SDI_MAINFRM_H


