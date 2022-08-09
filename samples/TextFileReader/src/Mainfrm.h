/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CMainFrame class for this sample
    program using the Win32++ Windows interface classes.

     Programming Notes: The programming style roughly follows that established
     got the 1995-1999 Jet Propulsion Laboratory Network Planning and
     Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H

/******************************************************************************/


/*============================================================================*/
    class
CMainFrame : public CFrame                                                  /*

    This class administers the user interaction with the file to be displayed
    and the view class that displays it. Parameters of this class deemed to
    be persistent may be serialized.
*-----------------------------------------------------------------------------*/
{
    public:
        CMainFrame(void);
        virtual     ~CMainFrame(){}
        virtual     HWND Create(HWND parent = 0);
        CDoc&       TheDoc()      {return m_view.TheDoc();}

        AboutBox&   GetAboutBox()   { return m_aboutDialog; }
        void        SaveSettings();
        void        SetArchivePath(const CString& archivePath)
                        { m_arcvPath = archivePath; }
        void        SetMaxMRUSlots(UINT maxMRU) { m_maxMRU = maxMRU; }

    protected:
        void        OnClose();
        void        OnColorChoice();
        BOOL        OnCommand(WPARAM wparam, LPARAM lparam);
        int         OnCreate(CREATESTRUCT& cs);
        void        OnFileClose();
        void        OnFileExit();
        void        OnFileOpenMRU(UINT);
        void        OnFileOpen();
        void        OnFontChoice() { m_view.ClientFontChoice();}
        BOOL        OnHelp();
        void        OnInitialUpdate()
                        { GetFrameMenu().CheckMenuItem(IDM_LINE_NUMBERING,
                            MF_UNCHECKED); }
        BOOL        OnProcessMRU(WPARAM wparam, LPARAM lparam);
        void        PreCreate(CREATESTRUCT& cs);
        void        Serialize(CArchive& ar);
        void        SetupMenuIcons();
        void        SetupToolBar();

    private:
        CMainFrame(const CMainFrame&);                // Disable copy construction
        CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

          // private data members
         CString    m_arcvPath;       // archived data path
         CView      m_view;           // the view
         UINT       m_xWin;           // deserialized window x position
         UINT       m_yWin;           // deserialized window y position
         UINT       m_cxWin;          // deserialized window width
         UINT       m_cyWin;          // deserialized window height
         UINT       m_maxMRU;         // limit on MRU entries
         AboutBox   m_aboutDialog;    // about dialog handled here
         WINDOWPLACEMENT m_wndPl;     // window placement information
};
/*------------------------------------------------------------------------------*/
#endif // SDI_MAINFRM_H

