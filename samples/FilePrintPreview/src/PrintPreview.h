/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]           (PrintPreview.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration and implementation of the DSize class and
    declaration of the CPreviewPaneEx, CPrintPreviewEx, and PreviewSetup classes
    using the Win32++ Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef PRINT_PREVIEW_H
#define PRINT_PREVIEW_H

#include "resource.h"
#include "PrintUtil.h"

class CPrintPreviewEx;
class CPreviewPaneEx;

/*=============================================================================*/
    class
DSize                                                                       /*

    A size class with double numeric entries. The features of this class
    are largely the same as those of CSize(int, int).
*-----------------------------------------------------------------------------*/
{
    public:
        DSize() {cx = cy = 0.0;}
        DSize(double x, double y) {cx = x; cy = y;}

        BOOL    operator==(const DSize& size) const
                    { return size.cx == cx && size.cy == cy;}
        BOOL    operator!=(const DSize& size) const
                    { return !(size == *this);}
        void    operator+=(DSize size)
                    { cx += size.cx; cy += size.cy; }
        void    operator-=(DSize size)
                    { cx -= size.cx; cy -= size.cy; }
        DSize   operator+(DSize size) const
                    { return DSize(cx + size.cx, cy + size.cy); }
        DSize   operator-(DSize size) const
                    { return DSize(cx - size.cx, cy - size.cy); }
        DSize   operator-() const
                    { return DSize(-cx, -cy); }

          // class data
        double  cx;
        double  cy;
};

/*============================================================================*/
    class
PreviewSetup    : public CDialog                                            /*

    The preview setup dialog class, which permits input of screen and
    initial preview window sizes, as well as particulars for the printer
    page.
*-----------------------------------------------------------------------------*/
{
    public:
        PreviewSetup() : CDialog(IDD_PREVIEW_SETUP) {}
        virtual ~PreviewSetup() {}

    protected:
        virtual void    DoDataExchange(CDataExchange& dx);
        CPrintPreviewEx& GetPreviewWnd()
                            { HWND h = ::GetParent(*this);
                              return *(CPrintPreviewEx*)GetCWndPtr(h);}
        virtual BOOL    OnInitDialog();
        virtual INT_PTR DialogProc(UINT msg, WPARAM, LPARAM);
        virtual BOOL    OnCommand(WPARAM, LPARAM);
        virtual BOOL    OnPageSetupButton();
        virtual void    OnCancel();
        virtual void    OnOK();

    private:
        BOOL    AddToolTip(UINT id)
                    { HWND h = GetDlgItem(id);
                      return m_toolTips.AddTool(h, id); }
        HWND    CreateToolTip(HWND dlg)
                    { HWND rtn = m_toolTips.Create(dlg);
                        m_toolTips.SetWindowTheme(L" ", L" "); // XP themes off
                        return rtn;}
        void    InitializeToolTips();

          // private data
        CDataExchange   m_dx;
        CButton         m_pageSetup;
        CToolTip        m_toolTips;
        DSize           m_inScreenInches;
        DSize           m_inPreviewInches;
};

/*=============================================================================*/
    class
CPreviewPaneEx : public CScrollView                                       /*

    A custom class that renders a scrollable bitmap in the client area of
    the CPrintPreviewEx dialog window.
*------------------------------------------------------------------------------*/
{
    public:
        CPreviewPaneEx();
        virtual ~CPreviewPaneEx() {}

        void    SetBitmap(CBitmap Bitmap) { m_bitmap = Bitmap; }
        void    SetPaneZoomState(int val)
                    { m_prevZoomState = m_zoomState;
                      m_zoomState = val;}
        void    ShowScrollBars(BOOL b)    {m_showScrollBars = b;}

    protected:
        CPrintPreviewEx&  GetPreviewWnd()
                            { HWND h = ::GetParent(*this);
                              return *(CPrintPreviewEx*)GetCWndPtr(h);}
        virtual void    OnDraw(CDC& dc);
        virtual BOOL    OnEraseBkgnd(CDC&);
        virtual LRESULT OnHScroll(UINT , WPARAM , LPARAM );
        virtual LRESULT OnMouseWheel(UINT , WPARAM , LPARAM );
        virtual LRESULT OnPaint(UINT , WPARAM , LPARAM );
        virtual LRESULT OnVScroll(UINT , WPARAM , LPARAM );

    private:
        DSize   GetZoom();

        BOOL    m_showScrollBars;
        CBitmap m_bitmap;
        int     m_zoomState;
        int     m_prevZoomState;
        CSize   m_scrollSize;
        CSize   m_prevWindowSize;
};

/*******************************************************************************

    Declaration of the CPrintPreviewEx class.

*=============================================================================*/
    class
CPrintPreviewEx : public CDialog                                         /*

    The print preview dialog window class.
*-----------------------------------------------------------------------------*/
{
    friend class PreviewSetup;

    public:
        CPrintPreviewEx(DWORD flags = HIDE_HELP);
        virtual ~CPrintPreviewEx() {}

        BOOL    ClosePreview();
        CPreviewPaneEx& GetPreviewPane()      {return m_previewPane;}
        DSize   GetPreviewSize() const        {return m_previewInches;}
        void    SetPreviewSize(DSize size)    {m_previewInches = size;}
        DSize   GetPrinterScreenRatio() const {return m_printerScreenRatio;}
        DSize   GetScreenSize() const         {return m_screenInches;}
        void    SetScreenSize(DSize size)     {m_screenInches = size;}
        void    InitializeContexts();
        BOOL    OnPreview(const CString&);
        void    SetSizes(DSize scrn, DSize prevw)
                    {m_screenInches = scrn; m_previewInches = prevw;}
        void    SetWindowSizes();

    protected:
        virtual void    DoDataExchange(CDataExchange& dx);
        virtual void    DocPageToBmp(UINT);
        virtual void    DoPreparePreview();
        virtual INT_PTR DialogProc(UINT msg, WPARAM, LPARAM);
        virtual void    LoadSizesRegistry();
        virtual BOOL    OnCommand(WPARAM, LPARAM);
        virtual BOOL    OnInitDialog();
        virtual void    OnOK();
        virtual void    OnPrepareDC();
        virtual void    OnPreparePrintPreview();
        virtual void    OnPreviewPage(UINT);
        virtual void    SaveSizesRegistry();

    private:
        BOOL    AddToolTip(UINT id)
                    { HWND h = GetDlgItem(id);
                      return m_toolTips.AddTool(h, id); }
        HWND    CreateToolTip(HWND dlg)
                    { HWND rtn = m_toolTips.Create(dlg);
                      m_toolTips.SetWindowTheme(L" ", L" "); // XP themes off
                      return rtn;}
        void    InitializeControls();
        void    InitializeToolTips();
        BOOL    OnFirstButton();
        BOOL    OnLastButton();
        BOOL    OnNextButton();
        BOOL    OnPrevButton();
        BOOL    OnPreviewHelp();
        BOOL    OnPrintButton();
        BOOL    OnZoomChange();
        void    PopulateScaleBox();
        BOOL    PreviewAndPageSetup();
        void    PreviewPage(UINT nPage);
        CString RegQueryStringValue(CRegKey &key, LPCTSTR name);
        void    UpdateButtons();

        CDataExchange   m_dx;
          // preview dialog controls
        CPreviewPaneEx m_previewPane;         // preview window
        CButton     m_buttonPrint;
        CButton     m_buttonSetup;
        CButton     m_buttonFirst;
        CButton     m_buttonLast;
        CButton     m_buttonNext;
        CEdit       m_editPage;
        CButton     m_buttonPrev;
        CButton     m_buttonClose;
        CComboBox   m_comboZoom;
        CButton     m_buttonPvwHelp;
        CResizer    m_resizer;
        CToolTip    m_toolTips;
          // button bitmaps
        CBitmap     m_firstPage;
        CBitmap     m_prevPage;
        CBitmap     m_nextPage;
        CBitmap     m_lastPage;
          // screen, initial preview size, and print page setup
        PreviewSetup m_setupDlg;            // setup dialog
        DSize       m_screenInches;         // screen size, in inches
        DSize       m_previewInches;        // preview size, inches
        DSize       m_printerScreenRatio;   // PrinterPPI/ScreenPPI
        CSize       m_printerDots;          // printer size, in dots
        CSize       m_screenPixels;         // screen size, in pixels

          // preview particulars
        CString     m_docPath;              // previewed document path
        CDC         m_dcPrinter;            // printer context
        CMemDC      m_dcMem;                // memory context
        UINT        m_currentPage;          // page number, zero based
        UINT        m_numPreviewPages;      // total pages
        double      m_shrink;               // printer/screen ratio
        DWORD       m_flags;                // dialog invocation flags
};
/*----------------------------------------------------------------------------*/
#endif // PRINT_PREVIEW_H
