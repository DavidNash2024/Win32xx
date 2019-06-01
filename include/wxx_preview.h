///////////////////////////////////////
// PreviewDialog.h

#ifndef _WIN32XX_PREVIEWDIALOG_H_
#define _WIN32XX_PREVIEWDIALOG_H_

#include "wxx_wincore.h"
#include "wxx_dialog.h"
#include "wxx_printdialogs.h"
#include "default_resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPrintPreview creates a memory device context from the printer's device
// context. Instead of printing directly to the printer, we print to the
// memory DC instead.


// To use CPrintPreview, do the following:
// 1) Declare CPrintPreview as CMainFrame's member, specifying the source's
//    class as the template type. The source is where the PrintPage function
//    resides.
// 2) Use SetSource to specify to where to call the PrintPage function.
// 3) Declare a PrintPage function in the source for printing and previewing:
//     void  PrintPage(CDC& dc, UINT page);
// 4) Call DoPrintPreview(HWND ownerWindow, UINT maxPage = 1) to initiate the
//    print preview.
// 5) Create the preview window, and swap it into the frame's view.
// 6) Handle UWM_PREVIEWCLOSE to swap back to the default view when the preview
//    closes.

// CPrintPreview calls the view's PrintPage function. This is the same
// function used to print a specified page. When printing, the PrintPage uses
// the specified printer DC. When previewing we supply the memory DC for the
// printer to the PrintPage function instead.

// After CPrintPreview calls PrintPage, it extracts the bitmap and displays
// it in CPrintPreview's preview pane.


namespace Win32xx
{
    // template of the PrintPreview dialog (used in place of a resource script).
    static unsigned char previewTemplate[] =
{
    0x01,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x48,0x00,0x00,0x42,0x05,
    0x00,0x00,0x00,0x00,0x00,0x35,0x01,0xb2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
    0x00,0x90,0x01,0x00,0x01,0x4d,0x00,0x53,0x00,0x20,0x00,0x53,0x00,0x68,0x00,0x65,
    0x00,0x6c,0x00,0x6c,0x00,0x20,0x00,0x44,0x00,0x6c,0x00,0x67,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x50,0x04,0x00,0x02,0x00,0x32,
    0x00,0x0e,0x00,0x5c,0x00,0x00,0x00,0xff,0xff,0x80,0x00,0x50,0x00,0x72,0x00,0x69,
    0x00,0x6e,0x00,0x74,0x00,0x2e,0x00,0x2e,0x00,0x2e,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x50,0x38,0x00,0x02,0x00,0x32,
    0x00,0x0e,0x00,0x5d,0x00,0x00,0x00,0xff,0xff,0x80,0x00,0x50,0x00,0x72,0x00,0x65,
    0x00,0x76,0x00,0x20,0x00,0x50,0x00,0x61,0x00,0x67,0x00,0x65,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x50,0x6c,
    0x00,0x02,0x00,0x32,0x00,0x0e,0x00,0x5e,0x00,0x00,0x00,0xff,0xff,0x80,0x00,0x4e,
    0x00,0x65,0x00,0x78,0x00,0x74,0x00,0x20,0x00,0x50,0x00,0x61,0x00,0x67,0x00,0x65,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0x50,0xa0,0x00,0x02,0x00,0x32,0x00,0x0e,0x00,0x5f,0x00,0x00,0x00,0xff,
    0xff,0x80,0x00,0x43,0x00,0x6c,0x00,0x6f,0x00,0x73,0x00,0x65,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x50,0x00,
    0x00,0x12,0x00,0x35,0x01,0x9f,0x00,0x60,0x00,0x00,0x00,0x50,0x00,0x72,0x00,0x65,
    0x00,0x76,0x00,0x69,0x00,0x65,0x00,0x77,0x00,0x50,0x00,0x61,0x00,0x6e,0x00,0x65,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

	//////////////////////////////////////////
	// Declaration of the CPreviewPane class
	// CPreviewPane provides a preview pane to CPrintPreview
	class CPreviewPane : public CWnd
	{
	public:
		CPreviewPane();
		virtual ~CPreviewPane() {}

		void Render(CDC& dc);
		void SetBitmap(CBitmap bitmap) { m_bitmap = bitmap; }
		void UseHalfTone(BOOL useHalfTone) { m_useHalfTone = useHalfTone; }

	protected:
		virtual void OnDraw(CDC& dc);
		virtual BOOL OnEraseBkgnd(CDC&);
		virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);

	private:
		CBitmap m_bitmap;
		BOOL m_useHalfTone;
	};


    //////////////////////////////////////////
    // Declaration of the CPrintPreview class
    //
    template <typename T>
    class CPrintPreview : public CDialog
    {
    public:
        CPrintPreview();
        virtual ~CPrintPreview();

    public:
        CPreviewPane& GetPreviewPane()  { return m_previewPane; }
        void DoPrintPreview(HWND ownerWindow, UINT maxPage = 1);
        BOOL OnPrintButton();
        BOOL OnNextButton();
        BOOL OnPrevButton();
        BOOL OnCloseButton();
        void PreviewPage(UINT page);
        void UseHalfTone(BOOL useHalfTone) { m_previewPane.UseHalfTone(useHalfTone); }
        void SetSource(T& source) { m_pSource = &source; }
        void UpdateButtons();

    protected:
        virtual BOOL OnInitDialog();
        virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
        virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);

    private:
        CPreviewPane m_previewPane;
        CResizer m_resizer;
        T*      m_pSource;
        CButton m_buttonPrint;
        CButton m_buttonNext;
        CButton m_buttonPrev;
        CButton m_buttonClose;
        UINT    m_currentPage;
        UINT    m_maxPage;
        HWND    m_ownerWindow;
    };

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

    //////////////////////////////////////////////////////
    // Definitions for the CPreviewPane class
    // CPreviewPane provides a preview pane for CPrintPreview
    //
    inline CPreviewPane::CPreviewPane() : m_useHalfTone(FALSE)
    {
        // The entry for the dialog's control in resource.rc must match this name.
        CString className = _T("PreviewPane");

        // Register the window class for use as a custom control in the dialog
        WNDCLASS wc;
        ZeroMemory(&wc, sizeof(WNDCLASS));

        if (!::GetClassInfo(GetApp()->GetInstanceHandle(), className, &wc))
        {
            wc.lpszClassName = className;
            wc.lpfnWndProc = ::DefWindowProc;
            wc.hInstance = GetApp()->GetInstanceHandle();
            wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
            wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
            ::RegisterClass(&wc);
        }

        assert(::GetClassInfo(GetApp()->GetInstanceHandle(), className, &wc));
    }

	inline BOOL CPreviewPane::OnEraseBkgnd(CDC&)
    {
        // prevent default erasure of the background
        return TRUE;
    }

    inline void CPreviewPane::OnDraw(CDC& dc)
    {
        Render(dc);
    }

    inline LRESULT CPreviewPane::OnPaint(UINT, WPARAM, LPARAM)
    // Normally OnDraw is suppressed for controls, but we need OnDraw for this window.
    {
        if (::GetUpdateRect(*this, NULL, FALSE))
        {
            CPaintDC dc(*this);
            OnDraw(dc);
        }
        else
            // RedrawWindow can require repainting without an update rect
        {
            CClientDC dc(*this);
            OnDraw(dc);
        }

        // No more drawing required
        return 0;
    }

    inline void CPreviewPane::Render(CDC& dc)
    // Copies the bitmap (m_Bitmap) to the PreviewPane, scaling the image
    //  to fit the window.
    {
        if (m_bitmap.GetHandle())
        {
            CMemDC memDC(dc);
            memDC.SelectObject(m_bitmap);
            BITMAP bm = m_bitmap.GetBitmapData();
            int border = 10;
            CRect rcClient = GetClientRect();

            double ratio = double(bm.bmHeight) / double(bm.bmWidth);
            int previewWidth;
            int previewHeight;

            // These borders center the preview with the PreviewPane.
            int xBorder = border;
            int yBorder = border;

            if ((rcClient.Width() - 2 * border)*ratio < (rcClient.Height() - 2 * border))
            {
                previewWidth = rcClient.Width() - (2 * border);
                previewHeight = static_cast<int>(previewWidth * ratio);

                yBorder = (rcClient.Height() - previewHeight) / 2;
            }
            else
            {
                previewHeight = rcClient.Height() - (2 * border);
                previewWidth = static_cast<int>(previewHeight / ratio);

                xBorder = (rcClient.Width() - previewWidth) / 2;
            }

            // Copy from the memory dc to the PreviewPane's DC with stretching.
            if (m_useHalfTone != FALSE)
            {
                dc.SetStretchBltMode(HALFTONE);         // provides smoother bitmap rendering
                ::SetBrushOrgEx(dc, xBorder, yBorder, NULL);
            }
            dc.StretchBlt(xBorder, yBorder, previewWidth, previewHeight, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

            // Draw a grey border around the preview
            // OnEraseBkgnd suppresses background drawing to avoid flicker on Windows XP and earlier
            CRect rcFill(0, 0, xBorder, previewHeight + yBorder);
            dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));

            rcFill.SetRect(0, 0, previewWidth + xBorder, yBorder);
            dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));

            rcFill.SetRect(previewWidth + xBorder, 0, rcClient.Width(), rcClient.Height());
            dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));

            rcFill.SetRect(0, previewHeight + yBorder, rcClient.Width(), rcClient.Height());
            dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));
        }
    }

    ///////////////////////////////////////////
    // Definitions for the CPrintPreview class
    //
    template <typename T>
    inline CPrintPreview<T>::CPrintPreview() : CDialog((LPCDLGTEMPLATE)previewTemplate),
		m_pSource(0), m_currentPage(0), m_maxPage(1), m_ownerWindow(0)
    {
    }

    template <typename T>
    inline CPrintPreview<T>::~CPrintPreview()
    {
    }

    template <typename T>
    inline INT_PTR CPrintPreview<T>::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        // Pass resizing messages on to the resizer
        m_resizer.HandleMessage(msg, wparam, lparam);

        //  switch (msg)
        //  {
                //Additional messages to be handled go here
        //  }

            // Pass unhandled messages on to parent DialogProc
        return DialogProcDefault(msg, wparam, lparam);
    }

    template <typename T>
    inline BOOL CPrintPreview<T>::OnCloseButton()
    // Called when the close button is pressed.
    {
        ::SendMessage(m_ownerWindow, UWM_PREVIEWCLOSE, 0, 0);
        return TRUE;
    }

    template <typename T>
    inline BOOL CPrintPreview<T>::OnCommand(WPARAM wparam, LPARAM lparam)
    // Processes the dialog's buttons.
    {
        UNREFERENCED_PARAMETER(lparam);

        UINT id = LOWORD(wparam);
        switch (id)
        {
        case IDW_PREVIEWPRINT:   return OnPrintButton();
        case IDW_PREVIEWPREV:    return OnPrevButton();
        case IDW_PREVIEWNEXT:    return OnNextButton();
        case IDW_PREVIEWCLOSE:   return OnCloseButton();
        }

        return FALSE;
    }

    template <typename T>
    inline BOOL CPrintPreview<T>::OnInitDialog()
    {
        // Attach the dialog controls to CWnd objects
        AttachItem(IDW_PREVIEWPRINT, m_buttonPrint);
        AttachItem(IDW_PREVIEWPREV, m_buttonPrev);
        AttachItem(IDW_PREVIEWNEXT, m_buttonNext);
        AttachItem(IDW_PREVIEWCLOSE, m_buttonClose);
        AttachItem(IDW_PREVIEWPANE, m_previewPane);

        // Support dialog resizing
        m_resizer.Initialize(*this, CRect(0, 0, 100, 120));
        m_resizer.AddChild(m_buttonPrint, topleft, 0);
        m_resizer.AddChild(m_buttonPrev, topleft, 0);
        m_resizer.AddChild(m_buttonNext, topleft, 0);
        m_resizer.AddChild(m_buttonClose, topleft, 0);
        m_resizer.AddChild(m_previewPane, topleft, RD_STRETCH_WIDTH | RD_STRETCH_HEIGHT);

        m_currentPage = 0;
        return TRUE;
    }

    template <typename T>
    inline BOOL CPrintPreview<T>::OnNextButton()
    // Called when the Next Page button is pressed.
    {
        PreviewPage(++m_currentPage);
        UpdateButtons();

        return TRUE;
    }

    template <typename T>
    inline BOOL CPrintPreview<T>::OnPrevButton()
    // Called when the Prev Page button is pressed.
    {
        PreviewPage(--m_currentPage);
        UpdateButtons();

        return TRUE;
    }

    template <typename T>
    inline BOOL CPrintPreview<T>::OnPrintButton()
    // Called when the Print button is pressed.
    // Sends the UWM_PRINTNOW message to the owner window.
    {
        ::SendMessage(m_ownerWindow, UWM_PRINTNOW, 0, 0);
        return TRUE;
    }

    template <typename T>
    inline void CPrintPreview<T>::DoPrintPreview(HWND ownerWindow, UINT maxPage)
    // Initiate the print preview.
    // ownerWindow: Print Preview's notifications are sent to this window.
    {
        m_ownerWindow = ownerWindow;
        assert(maxPage >= 1);
        m_maxPage = maxPage;

        // Preview the first page;
        m_currentPage = 0;
        PreviewPage(0);
    }

    template <typename T>
    inline void CPrintPreview<T>::PreviewPage(UINT page)
    // Preview's the specified page.
    // This function calls the view's PrintPage function to render the same
    // information that would be printed on a page.
    {
        // Get the device contect of the default or currently chosen printer
        CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
        CDC printerDC = printDlg.GetPrinterDC();

        // Create a memory DC for the printer.
        // Note: we use the printer's DC here to render text accurately
        CMemDC memDC(printerDC);

        // Create a compatible bitmap for the memory DC
        int width = printerDC.GetDeviceCaps(HORZRES);
        int height = printerDC.GetDeviceCaps(VERTRES);

        // A bitmap to hold all the pixels of the printed page would be too large.
        // Shrinking its dimensions by 4 reduces it to 1/16th its original size.
        int shrink = width > 10000 ? 8 : 4;

        CDC previewDC = GetPreviewPane().GetDC();
        memDC.CreateCompatibleBitmap(printerDC, width / shrink, height / shrink);

        memDC.SetMapMode(MM_ANISOTROPIC);
        memDC.SetWindowExtEx(width, height, NULL);
        memDC.SetViewportExtEx(width / shrink, height / shrink, NULL);

        // Fill the bitmap with a white background
        CRect rc(0, 0, width, height);
        memDC.FillRect(rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));

        // Call PrintPage from the source.
		assert(m_pSource);
        m_pSource->PrintPage(memDC, page);

        // Detach the bitmap from the memory DC and save it
        CBitmap bitmap = memDC.DetachBitmap();
        GetPreviewPane().SetBitmap(bitmap);

        // Display the print preview
        UpdateButtons();
        GetPreviewPane().Render(previewDC);
    }

    template <typename T>
    inline void CPrintPreview<T>::UpdateButtons()
    // Enables or disables the page selection buttons.
    {
        m_buttonNext.EnableWindow(m_currentPage < m_maxPage - 1);
        m_buttonPrev.EnableWindow(m_currentPage > 0);
    }

}


#endif // _WIN32XX_PREVIEWDIALOG_H_
