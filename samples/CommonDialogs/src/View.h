/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                   (View.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the  CView class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes. This particular view class utilizes a rich edit control to
    contain, view, and manage the document'd data.  It also  applies methods
    for setting the view's foreground and background colors and the edit box
    font. It also demonstrates serialization of class parameters in an external
    file.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

   // Identifiers of entries in the m_colorTable array.
enum CtlColors
{
    DfltClr = 0,             // default color
    DlgBg,                   // dialog background
    REdTxFg,                 // rich edit text foreground
    REdTxBg,                 // rich edit text background
    REdBg,                   // rich edit control background
    SBBg,                    // status bar
    EndColors                // end color IDs
};

/*============================================================================*/
    class
CView : public CDialog                                                      /*

    This application's View class, a pattern for developing new apps.
*-----------------------------------------------------------------------------*/
{
    public:
        CView(UINT id);
        virtual ~CView(){}

        HWND        Create(HWND);
        CRichEditView& GetRichView() { return m_richView;}
        CFont       GetEditFont()    { return m_editFont;}
        void        NoDocOpen();
        void        OnPageSetup();
        void        OnPrintDocument(LPCTSTR);
        void        SetEditFont(const CFont f);
        void        SetBgColor(CBrush br) { m_bgBrush = br;}
        void        SetRichEditColors(COLORREF, COLORREF, COLORREF);

    private:
        CView(const CView&);                // Disable copy construction
        CView& operator = (const CView&);   // Disable assignment operator

        BOOL        AddToolTip(HWND, UINT id);
        BOOL        AddToolTip(HWND, UINT id, LPCTSTR s);
        void        AssignToolTips();
        INT_PTR     DialogProc(UINT, WPARAM, LPARAM);
        void        OnCancel() {}  // Suppress esc key closing the dialog
        void        OnClose() {}   // Suppress esc key closing the dialog
        INT_PTR     OnCtlColor(UINT, WPARAM, LPARAM);
        BOOL        OnInitDialog();
        LRESULT     OnNotify(WPARAM wparam, LPARAM lparam);
        void        Serialize(CArchive &ar);

        HWND            m_parent;       // handle of parent frame
        CToolTip        m_toolTip;      // form tool tips
        CFont           m_editFont;     // edit box font
        CBrush          m_bgBrush;      // backbround brush object
        CRichEditView   m_richView;     // the view of the document
};
/*----------------------------------------------------------------------------*/
#endif // SDI_VIEW_H
