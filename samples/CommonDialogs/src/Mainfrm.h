/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CMainFrame class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes. This particular frame class contains features a fixed-size form
    for the display, with no resizing gripper tool at the end of the status bar,
    and provisions for selection of client background color, selection of
    edit box font, use of external serialization files, and MRU lists.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H

  // designation of where a control id appears, used when enabling/disabling
enum ControlBars {toolbar, mainmenu, both};

/*============================================================================*/
    class
CMainFrame : public CFrame                                                  /*

    This application's mainframe class.
*-----------------------------------------------------------------------------*/
{
    public:
        CMainFrame();
        virtual ~CMainFrame() {}

        void    AddMRUEntry(LPCTSTR MRUEntry)
                    { CFrame::AddMRUEntry (MRUEntry);}
        virtual HWND Create(HWND parent = 0);
        AboutBox& GetAboutBox() { return m_aboutBox; }
        void    EmptyMRUList();
        size_t  GetMRUSize() { return GetMRUEntries().size();}
        COLORREF GetSBBkColor()
                    { return m_colorChoice.GetTableColor(SBBg);}
        void    SetSBBkColor(COLORREF clr)
                    { GetStatusBar().SendMessage(SB_SETBKCOLOR,
                        0, (LPARAM)clr);}
        void    SetArchivePath(LPCTSTR path) { m_archivePath = path; }
        void    SetDocExt(LPCTSTR ext)       { m_docExt = ext; }
        void    SetDocFilter(LPCTSTR filter) { m_docFilter = filter; }
        void    SetMaxMRU(UINT max)          { m_maxMRU = max; }
        void    SetWindowTitle(LPCTSTR);
        void    UpdateControlUIState();

        LRESULT OnActivate(UINT, WPARAM wparam, LPARAM lparam)
                    { CWnd::WndProcDefault(WM_ACTIVATE, wparam, lparam);
                      if (LOWORD(wparam) == WA_ACTIVE) GetRichView().SetFocus();
                      return 0; }

    private:
        CMainFrame(const CMainFrame&);                // Disable copy construction
        CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

        BOOL    DropFiles(LPARAM lparam);
        CRichEditView& GetRichView() { return m_view.GetRichView();}
        void    InitCtlColors();
        void    LoadPersistentData();
        void    OnCloseDoc();
        void    OnColorChoice();
        BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
        int     OnCreate(CREATESTRUCT& rcs);
        void    OnEditFind();
        void    OnEditReplace();
        void    OnFileOpenMRU(UINT);
        void    OnFontChoice();
        BOOL    OnHelp();
        void    OnInitialUpdate();
        LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
        void    OnNewDoc();
        void    OnOpenDoc();
        void    OnSaveAs();
        BOOL    OnProcessMRU(WPARAM wparam, LPARAM lparam);
        void    OnWrapText();
        BOOL    OpenDoc(LPCTSTR);
        void    PreCreate(CREATESTRUCT& cs);
        BOOL    SaveRegistrySettings();
        void    Serialize(CArchive &ar);
        BOOL    SetCheckStatus(UINT, BOOL, ControlBars);
        BOOL    SetEnableStatus(UINT, BOOL, ControlBars);
        void    SetReBarColors(COLORREF, COLORREF, COLORREF, COLORREF);
        BOOL    SetRichEditColor();
        void    SetStatusbarMsg(CString);
        BOOL    SetThemeColors();
        void    SetupMenuIcons();
        void    SetupToolBar();
        void    SetViewBgColor()
                     {m_view.SetBgColor(m_colorChoice.GetBrush(DlgBg));}
        void    UpdateMRUMenu();
        LRESULT WndProc(UINT msg, WPARAM, LPARAM);
        void    ValidateMRU();

        CDoc         m_doc;         // the document
        CView        m_view;        // the view
        AboutBox     m_aboutBox;    // the AboutBox dialog
        CString      m_archivePath, // archive file
                     m_docDir,      // for the file open/save dialogs
                     m_docExt,      // document file extension
                     m_docFilter;   // document file open/save filter
        UINT         m_maxMRU;      // maximum MRU entries, this app
        CBitmap      m_colorbmp;    // for the color choice menu item
        BOOL         m_isTextWrap;  // wrap text in rich edit if true
        CColorChoice m_colorChoice; // the control color choice
        MyFontDialog m_fontChoice;  // edit control font
        CPoint       m_frameXY;     // frame top-left coordinates
        CSize        m_frameSize;   // frame dimensions
        MyFindReplaceDialog m_findReplaceDlg;  // find-replace dialog
};
/*------------------------------------------------------------------------------*/
#endif // SDI_MAINFRM_H

