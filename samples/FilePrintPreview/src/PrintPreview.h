/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]           (PrintPreview.h) *
********************************************************************************

    Declaration and implementation of the DSize class and declaration of 
    the CPreviewPane, CPrintPreview, and PreviewSetup classes.

********************************************************************************

    Acknowledgment. These classes were adapted from the PrintPreview sample 
    program appearing in the Win32++ framework sample folder, created by  
    David Nash and published under the permissions granted in that work.
    The adaptation here reimplements the PrintView window as a separate 
    popup window that appears on screen along with the regular program 
    window. This work has been developed under the co-authorship of Robert
    Tausworthe and David Nash, and released under the copyright provisions 
    of the Win32++ Interface Classes software, copyright (c) David Nash,
    2005-2018. The former author acknowledges and thanks the latter for his 
    patient direction and inspiration in the development of the classes of 
    these classes.

*******************************************************************************/

#ifndef PRINT_PREVIEW_H
#define PRINT_PREVIEW_H

#include "resource.h"
#include "PrintUtil.h"

class CPrintPreview;
class CPreviewPane;

/*=============================================================================*/
    class
DSize																	/*

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
        double  cx, 
            cy;
};

/*============================================================================*/
    class
PreviewSetup    : public CDialog										/*

    The preview setup dialog class, which permits input of screen and
    initial preview window sizes, as well as particulars for the printer
    page.
*-----------------------------------------------------------------------------*/
{
    public:
        PreviewSetup(UINT nResID);
        
    protected:
		virtual void	DoDataExchange(CDataExchange& DX);
        CPrintPreview&	GetPreviewWnd()
							{ HWND h = ::GetParent(*this);
							  return *(CPrintPreview*)GetCWndPtr(h);}
        virtual BOOL    OnInitDialog();
        virtual INT_PTR DialogProc(UINT uMsg, WPARAM, LPARAM);
        virtual BOOL    OnCommand(WPARAM, LPARAM);
        virtual BOOL    OnPageSetupButton();
        virtual void    OnCancel();
        virtual void    OnOK();

    private:
            BOOL    AddToolTip(UINT nID)
                    { HWND h = GetDlgItem(nID);
                      return m_TT.AddTool(h, nID); }
            HWND    CreateToolTip(HWND hDlg)
						{   HWND rtn = m_TT.Create(hDlg);
							  // Turn XP themes off
							m_TT.SetWindowTheme(L" ", L" ");
							return rtn;}
			void    InitializeToolTips();

          // private data
 		CDataExchange	m_DX;
		CButton			m_PageSetup;
        CToolTip		m_TT;
		DSize			m_InScreenInches,
						m_InPreviewInches;
};
    
/*=============================================================================*/
    class
CPreviewPane : public CScrollView										/*

    A custom class that renders a scrollable bitmap in the client area of
    the CPrintPreview dialog window.
*------------------------------------------------------------------------------*/
{
    public:
        CPreviewPane();
        virtual ~CPreviewPane() {}

            void    SetBitmap(CBitmap& Bitmap)
						{ m_Bitmap = Bitmap; }
            void    SetPaneZoomState(int val)
						{ m_PrevZoomState = m_ZoomState;
						  m_ZoomState = val;}
            void    ShowScrollBars(BOOL b) {m_ShowScrollBars = b;}

    protected:
        CPrintPreview&	GetPreviewWnd()
							{ HWND h = ::GetParent(*this);
							  return *(CPrintPreview*)GetCWndPtr(h);}
        virtual void    OnDraw(CDC& dc);
        virtual BOOL    OnEraseBkgnd(CDC&);
        virtual LRESULT OnHScroll(UINT , WPARAM , LPARAM );
        virtual LRESULT OnMouseWheel(UINT , WPARAM , LPARAM );
        virtual LRESULT OnPaint(UINT , WPARAM , LPARAM );
        virtual LRESULT OnVScroll(UINT , WPARAM , LPARAM );

    private:
        DSize   GetZoom();

		BOOL			m_ShowScrollBars;
        CBitmap			m_Bitmap;
        int				m_ZoomState,
						m_PrevZoomState;
        CSize			m_ScrollSize,
						m_LastWindowSize;
};

/*******************************************************************************

    Declaration of the CPrintPreview class.

*=============================================================================*/
    class
CPrintPreview : public CDialog                                         /*

    The print preview dialog window class.
*-----------------------------------------------------------------------------*/
{
    public:
        CPrintPreview(UINT nResID, DWORD dwFlags = HIDE_HELP);
        virtual ~CPrintPreview();

            BOOL		ClosePreview();
            CPreviewPane& GetPreviewPane() 
							{ return m_PreviewPane;}
            DSize		GetPreviewSize() 
							{return m_PreviewInches;}
            DSize		GetPrinterScreenRatio() 
							{ return m_PrinterScreenResRatio;}
            DSize		GetScreenSize() 
							{return m_ScreenInches;}
            void		InitializeContexts();
            BOOL		OnPreview(const CString&); 
  			void		SetSizes(DSize scrn, DSize prevw)
							{ m_ScreenInches = scrn; m_PreviewInches = prevw;}

		friend class PreviewSetup;

    protected:
		virtual void	DoDataExchange(CDataExchange& DX);
        virtual void    DocPageToBmp(UINT);
        virtual void    DoPreparePreview();
        virtual INT_PTR DialogProc(UINT uMsg, WPARAM, LPARAM);
        virtual void    LoadSizesRegistry();
        virtual BOOL    OnCommand(WPARAM, LPARAM);
        virtual BOOL    OnInitDialog();
        virtual void    OnOK();
        virtual void    OnPrepareDC();
        virtual void    OnPreparePrintPreview();
        virtual void    OnPreviewPage(UINT);
        virtual void    SaveSizesRegistry();

          // preview particulars
        CString     m_sDocPath;				// previewed document path
        CDC			m_dcPrinter;			// printer context
        CMemDC      m_dcMem;				// memory context
        CPreviewPane m_PreviewPane;			// preview window
        UINT        m_nCurrentPage,			// page number, zero based
					m_nNumPreviewPages;		// total pages
		double		m_shrink;				// printer/screen ratio

    private:
        BOOL    AddToolTip(UINT nID)
					{ HWND h = GetDlgItem(nID);
					  return m_TT.AddTool(h, nID); }
        HWND    CreateToolTip(HWND hDlg)
					{ HWND rtn = m_TT.Create(hDlg);
					    // Turn XP themes off
					  m_TT.SetWindowTheme(L" ", L" ");
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
		void	PopulateScaleBox();
        BOOL    PreviewAndPageSetup();
        void    PreviewPage(UINT nPage);
        CString RegQueryStringValue(CRegKey &key, LPCTSTR pName);
        void    SetWindowSizes();
        void    UpdateButtons();

		CDataExchange	m_DX;
          // preview dialog controls
        CButton     m_ButtonPrint;
        CButton     m_ButtonSetup;
        CButton     m_ButtonFirst;
        CButton     m_ButtonLast;
        CButton     m_ButtonNext;
        CEdit       m_EditPage;
        CButton     m_ButtonPrev;
        CButton     m_ButtonClose;
        CComboBox   m_ComboZoom;
        CButton     m_ButtonPvwHelp;
        CResizer    m_Resizer;
        CToolTip    m_TT;
        DWORD       m_dwFlags;				// dialog invocation flags
          // button bitmaps
        CBitmap     m_FirstPage,
					m_PrevPage,
					m_NextPage,
		            m_LastPage;
          // screen, initial preview size, and print page setup
        PreviewSetup m_SetupDlg;			// setup dialog
        DSize       m_ScreenInches,			// screen size, in inches
					m_PreviewInches,		// preview size, inches
					m_PrinterScreenResRatio; // PrinterPPI/ScreenPPI
        CSize       m_PrinterDots,			// printer size, in dots
					m_ScreenPixels;			// screen size, in pixels
};
/*----------------------------------------------------------------------------*/
#endif // PRINT_PREVIEW_H