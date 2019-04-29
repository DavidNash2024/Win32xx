/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of a basic CMainFrame class for the
    ScrollWin demo application using the Win32++ Windows interface classes,
    Copyright (c) 2005-2017 David Nash, under permissions granted therein.
    
    Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H

/******************************************************************************/


  // designation of where a control id appears, used when enabling/disabling
enum ControlBars {toolbar, mainmenu, both};

/*============================================================================*/
    class 
CMainFrame : public CFrame                                                  /*

*-----------------------------------------------------------------------------*/
{
    public:
        CMainFrame(void);
        virtual       ~CMainFrame();
        CDoc&         TheDoc() {return m_Doc;}
        virtual void  Serialize(CArchive& ar);

        // public data members
        static   CString m_sCompiled_on; // compilation date, mmm dd yyyy

    protected:
        virtual void    OnColorChoice(void);
        virtual BOOL    OnCommand(WPARAM wParam, LPARAM lParam);
        virtual int     OnCreate(CREATESTRUCT& cs);
        virtual void    OnFileClose(void);
        virtual void    OnFileExit(void);
        virtual void    OnFileOpenMRU(UINT);
        virtual void    OnFileOpen(void);
        virtual void    OnFontChoice(void) { m_View.ClientFontChoice();}
        virtual BOOL    OnHelp(void);
        virtual void    OnInitialUpdate(void);
        virtual void    OnMenuUpdate(UINT nID);
        virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
        virtual BOOL    OnProcessMRU(WPARAM wParam, LPARAM lParam);
        virtual void    PreCreate(CREATESTRUCT& cs);
        virtual BOOL    PreTranslateMessage(MSG &Msg);
        virtual BOOL    SaveRegistrySettings(void);
        virtual void    SetupMenuIcons();
        virtual void    SetupToolBar(void);
        virtual LRESULT WndProc(UINT uMsg, WPARAM, LPARAM);

    private:
          // private data members
         CDoc       m_Doc;          // the document
         CView      m_View;         // the view
         UINT       m_win_x;        // deserialized window x position
         UINT       m_win_y;        // deserialized window y position
         UINT       m_win_width;    // deserialized window width
         UINT       m_win_height;   // deserialized window height
         UINT       m_nMaxMRU;      // limit on MRU entries
         AboutBox   m_AboutDialog;  // about dialog handled here
         WINDOWPLACEMENT m_Wndpl;   // window placement information

};

/*==============================================================================

    Declare the global function that provides access to the
    CDoc class                          */
    
CDoc& TheDoc();

/*------------------------------------------------------------------------------*/
#endif // SDI_MAINFRM_H

