///////////////////////////////////////
// PreviewDialog.cpp

#include "stdafx.h"
#include "PreviewDialog.h"
#include "TextApp.h"
#include "resource.h"


/////////////////////////////////////////
// Definitions for the CPreviewPane class
//
CPreviewPane::CPreviewPane()
{
    // The entry for the dialog's control in resource.rc must match this name.
    CString className = _T("PreviewPane");

    // Register the window class for use as a custom control in the dialog
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));

    if (!::GetClassInfo(GetApp().GetInstanceHandle(), className, &wc))
    {
        wc.lpszClassName = className;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.hInstance = GetApp().GetInstanceHandle();
        wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
        wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
        ::RegisterClass(&wc);
    }

    assert(::GetClassInfo(GetApp().GetInstanceHandle(), className, &wc));
}

BOOL CPreviewPane::OnEraseBkgnd(CDC& )
{
    // prevent default erasure of the background
    return TRUE; 
}


void CPreviewPane::OnDraw(CDC& dc)
{
    Render(dc);
}

LRESULT CPreviewPane::OnPaint(UINT, WPARAM, LPARAM)
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

void CPreviewPane::Render(CDC& dc)
// Copies the bitmap (m_Bitmap) to the PreviewPane, scaling the image 
//  to fit the window.
{
    if (m_bitmap.GetHandle())
    {
        CMemDC memDC(dc);
        memDC.SelectObject(m_bitmap);
        BITMAP bm = m_bitmap.GetBitmapData();
        int border = 20;
        CRect rcClient = GetClientRect();

        double ratio = double(bm.bmHeight) / double(bm.bmWidth);
        int previewWidth;
        int previewHeight;
        
        // These borders center the preview with the PreviewPane.
        int xBorder = border;
        int yBorder = border;

        if ((rcClient.Width() - 2*border)*ratio < (rcClient.Height() - 2*border))
        {
            previewWidth = rcClient.Width() - (2*border);
            previewHeight = static_cast<int>(previewWidth * ratio);

            yBorder = (rcClient.Height() - previewHeight) / 2;
        }
        else
        {
            previewHeight = rcClient.Height() - (2*border);
            previewWidth = static_cast<int>(previewHeight / ratio);

            xBorder = (rcClient.Width() - previewWidth) / 2;
        }

        // Copy from the memory dc to the PreviewPane's DC with stretching.     
        dc.SetStretchBltMode(HALFTONE);         // provides smoother bitmap rendering
        ::SetBrushOrgEx(dc, xBorder, yBorder, NULL);
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
// Definitions for the CPreviewDialog class
//
CPreviewDialog::CPreviewDialog(UINT resID) : CDialog(resID), m_currentPage(0)
{
}

CPreviewDialog::~CPreviewDialog()
{
}

CRichEdit& CPreviewDialog::GetRichView()
{
    CMainFrame& MainFrame = GetTextApp().GetMainFrame();
    return MainFrame.GetRichView();
}

INT_PTR CPreviewDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
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

BOOL CPreviewDialog::OnCloseButton()
{
    GetAncestor().SendMessage(UWM_CHANGEVIEW);
    return TRUE;
}

BOOL CPreviewDialog::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDC_BUTTONPRINT:   return OnPrintButton();
    case IDC_BUTTONPREV:    return OnPrevButton();
    case IDC_BUTTONNEXT:    return OnNextButton();
    case IDC_BUTTONCLOSE:   return OnCloseButton();
    }

    return FALSE;
}

BOOL CPreviewDialog::OnInitDialog()
{
    // Attach the dialog controls to CWnd objects
    AttachItem(IDC_BUTTONPRINT, m_buttonPrint);
    AttachItem(IDC_BUTTONPREV, m_buttonPrev);
    AttachItem(IDC_BUTTONNEXT, m_buttonNext);
    AttachItem(IDC_BUTTONCLOSE, m_buttonClose);
    AttachItem(IDC_CUSTOM1, m_previewPane);

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

BOOL CPreviewDialog::OnNextButton()
{
    PreviewPage(++m_currentPage);
    UpdateButtons();

    return TRUE;
}

BOOL CPreviewDialog::OnPrevButton()
{
    PreviewPage(--m_currentPage);
    UpdateButtons();
    
    return TRUE;
}

BOOL CPreviewDialog::OnPrintButton()
{
    GetAncestor().SendMessage(UWM_PRINTNOW);
    return TRUE; 
}

BOOL CPreviewDialog::DoPrintPreview(RECT page, RECT printArea)
{
    m_pageRect = page;
    m_printArea = printArea;

    // Get the device contect of the default or currently chosen printer
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    CDC printerDC = printDlg.GetPrinterDC();
    printerDC.SetMapMode(MM_TEXT);

    FORMATRANGE fr;
    fr.hdcTarget = printerDC;
    fr.hdc = printerDC;
    fr.rcPage = page;
    fr.rc = printArea;
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;

    // Find out real size of document in characters.
    LONG lTextLength;   // Length of document.
    lTextLength = GetRichView().GetTextLengthEx(GTL_NUMCHARS);

    // Calculate the page breaks
    LONG textPrinted;  // Amount of document printed.
    m_pageBreaks.clear();
    do
    {
        textPrinted = GetRichView().FormatRange(fr, FALSE);

        if (textPrinted < lTextLength)
        {
            fr.chrg.cpMin = textPrinted;
            fr.chrg.cpMax = -1;
            m_pageBreaks.push_back(textPrinted);
        }
    } while (textPrinted < lTextLength);

    m_pageBreaks.push_back(-1);

    // Clear the cache
    GetRichView().FormatRange();

    // Preview the first page;
    m_currentPage = 0;
    PreviewPage(0);

    return TRUE;
}

void CPreviewDialog::PreviewPage(UINT page)
{
    assert(m_pageBreaks.size() > 0);
    assert(page < m_pageBreaks.size());

    // Get the device contect of the default or currently chosen printer
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    CDC printerDC = printDlg.GetPrinterDC();
    
    // Create a memory DC for the printer.
    // Note: we use the printer's DC here to render text accurately
    CMemDC memDC(printerDC);

    // Assign values to the FORMATRANGE struct
    FORMATRANGE fr;
    ZeroMemory(&fr, sizeof(fr));
    fr.hdcTarget = printerDC;
    fr.hdc = memDC;
    fr.rcPage = m_pageRect;
    fr.rc = m_printArea;
    fr.chrg.cpMin = (page > 0) ? m_pageBreaks[page - 1] : 0;
    fr.chrg.cpMax = m_pageBreaks[page];

    // Create a compatible bitmap for the memory DC
    int width = printerDC.GetDeviceCaps(HORZRES);
    int height = printerDC.GetDeviceCaps(VERTRES);

    // A bitmap to hold all the pixels of the printed page would be too large.
    // Shrinking its dimensions by 4 reduces it to 1/16th its original size.
    int shrink = width > 10000? 8 : 4;
    
    CDC previewDC = GetPreviewPane().GetDC();
    memDC.CreateCompatibleBitmap(previewDC, width / shrink, height / shrink);

    memDC.SetMapMode(MM_ANISOTROPIC);
    memDC.SetWindowExtEx(width, height, NULL);
    memDC.SetViewportExtEx( width / shrink, height / shrink, NULL);

    // Fill the bitmap with a white background
    CRect rc(0, 0, width, height);
    memDC.FillRect(rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));

    // Display text from the richedit control on the memory dc
    GetRichView().FormatRange(fr, TRUE);

    // Tell the control to release the cached information.
    GetRichView().FormatRange();

    // Detach the bitmap from the memory DC and save it
    CBitmap bitmap = memDC.DetachBitmap();
    GetPreviewPane().SetBitmap(bitmap);

    // Display the print preview
    UpdateButtons();
    GetPreviewPane().Render(previewDC);
}

void CPreviewDialog::UpdateButtons()
{
    m_buttonNext.EnableWindow(m_currentPage < m_pageBreaks.size() - 1);
    m_buttonPrev.EnableWindow(m_currentPage > 0);
}

