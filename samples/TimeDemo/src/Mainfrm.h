/* (06-May-2024) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
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
#include "MRU.h"


/*============================================================================*/
    class
CMainFrame : public CFrame                      /*

    The application mainframe class that creates the main menu, toolbar,
    status bar, and CView display, and responds to user interactions.
*-----------------------------------------------------------------------------*/
{
    public:
                    CMainFrame();
        virtual     ~CMainFrame() override {}
        void        SetAppName(const CString& appName)
                                    { m_appName = appName; }
        void        SetArcFileName(const CString& arcName)
                                    { m_arcName = arcName; }
        void        SetWindowTitle(const CString &title = L"");
        CView&      TheView()       {return m_view;}
        CDoc&       ThisDoc()       {return m_view.TheDoc();}
        CMRU&       TheMRU()        {return m_MRU;};
        AboutBox&   GetAboutBox()   { return m_aboutDialog; }

        static const CString m_compiledOn; // compilation date

    protected:

        virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
        virtual int     OnCreate(CREATESTRUCT& cs) override;
        virtual void    OnDestroy() override;
        virtual void    OnInitialUpdate() override;
        virtual void    PreCreate(CREATESTRUCT& cs) override;
        virtual void    Serialize(CArchive &ar) override;
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
        bool    OnFileOpenMRU(UINT);
        void    OnFileSave();
        void    OnFileSaveAs();
        void    OnFontChoice();
        BOOL    OnHelpAbout();
        BOOL    OnProcessMRU(WPARAM wparam, LPARAM lparam);

          // private data members
        AboutBox   m_aboutDialog;  // about dialog object
        CMRU       m_MRU;          // the MRU list object
        CView      m_view;         // the view object

        CString    m_appName;      // application name
        CString    m_arcName;      // archive file name

        UINT       m_xWin;        // deserialized window x position
        UINT       m_yWin;        // deserialized window y position
        UINT       m_cxWin;       // deserialized window width
        UINT       m_cyWin;       // deserialized window height
        WINDOWPLACEMENT m_plWnd;  // window placement information

        const size_t m_maxMRUEntries;
};
/*----------------------------------------------------------------------------*/
#endif // SDI_MAINFRM_H


