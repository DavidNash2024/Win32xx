/* (26-Mar-2025)                                             (CRichEditView.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CRichEditView class for the
    CommonDialogs sample application using the Win32++ Windows interface
    class. This class serves both as the document view and content manager.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"


CRichEditView::CRichEditView() : m_textLength(0), m_isAppBanded(FALSE)
{
    m_fr = {};
}

// Clear the control of all text.
void CRichEditView::Clean()
{
    SetWindowText(L"");
}

// Return TRUE if text is currently selected.
BOOL CRichEditView::IsSelected()
{
    CHARRANGE r;
    GetSel(r);
    return (r.cpMin != r.cpMax);
}

void CRichEditView::OnAttach()
{
    // Increase the text limit of the rich edit window: here, (DWORD)-1,
    // or the maximum DWORD value.
    LimitText(-1);

    // Determine which messages will be passed to the parent.
    DWORD dwMask = ENM_KEYEVENTS | ENM_DROPFILES | ENM_MOUSEEVENTS;
    SetEventMask(dwMask);
}

// Set the rich edit control text foreground and background colors and the
// control background color.
void CRichEditView::SetColors(COLORREF txfg, COLORREF txbg, COLORREF bg)
{
    CHARFORMAT2 chf;
    chf.cbSize = sizeof(chf);
    chf.dwMask = CFM_COLOR | CFM_BACKCOLOR;
    chf.dwEffects = 0;
    chf.crTextColor = txfg;
    chf.crBackColor = txbg;
    SetDefaultCharFormat(chf);
    SetBackgroundColor(FALSE, bg);
}

// Set the display font; if nullptr, the system font is used. Immediately
// redraw the view if TRUE.
void CRichEditView::SetFont(HFONT font, BOOL redraw) const
{
    CRichEdit::SetFont(font, redraw);

    // Do not automatically change fonts when the user explicitly changes
    // to a different keyboard layout.
    LRESULT lres = SendMessage(EM_GETLANGOPTIONS, 0, 0);
    lres &= ~IMF_AUTOFONT;
    SendMessage(EM_SETLANGOPTIONS, 0, lres);
}

// Set the rich edit view window to use word wrapping(wrap is TRUE) or
// not use word wrapping(wrap is FALSE).
void CRichEditView::SetWrapping(int wrap)
{
    SetTargetDevice(nullptr, (wrap == 0 ? TRUE : FALSE));
}

// Read the text from the file into this rich edit control.
BOOL CRichEditView::StreamInFile(const CFile& file)
{
    UINT format = SF_TEXT;
    EDITSTREAM es;
    es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
    es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(
        reinterpret_cast<void*>(StreamInCallback));
    StreamIn(format, es);

    // Clear the modified text flag
    SetModify(FALSE);
    return TRUE;
}

// Write the text into the specified file.
BOOL CRichEditView::StreamOutFile(const CFile& file)
{
    UINT format = SF_TEXT;
    EDITSTREAM es;
    es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
    es.dwError = 0;
    es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(
        reinterpret_cast<void*>(StreamOutCallback));
    StreamOut(format, es);

    // Clear the modified text flag
    SetModify(FALSE);
    return TRUE;
}

// Replace the contents of the rich edit view window with data from an input
// stream. The parameter dwCookie holds the file handle. The pbuff parameter
// points to the stream's buffer. The number of bytes streamed in is stored
// in pch.
DWORD CALLBACK CRichEditView::StreamInCallback(DWORD dwCookie, LPBYTE pbBuff,
    LONG cb, LONG *pcb)
{
    *pcb = 0;
    if (!::ReadFile((HANDLE)(DWORD_PTR) dwCookie, pbBuff, cb, (LPDWORD)pcb,
      nullptr))
        ::MessageBox(nullptr, L"StreamInFile Failed", L"", MB_OK);

    return 0;
}

// Transfer data out of the rich edit view window. The dwCookie parameter holds
// the file handle. The pbuff parameter points to the stream's buffer. The
// number of bytes streamed in is stored in pch.
DWORD CALLBACK CRichEditView::StreamOutCallback(DWORD dwCookie, LPBYTE pbBuff,
    LONG cb, LONG *pcb)
{
    *pcb = 0;
    if (!::WriteFile((HANDLE)(DWORD_PTR)dwCookie, pbBuff, cb, (LPDWORD)pcb,
        nullptr))
    {
        ::MessageBox(nullptr, L"StreamOutFile Failed", L"", MB_OK);
    }

    return 0;
}

// Call this function to invoke the print dialog box to create a printer device
// context using the values in the info structure.
BOOL CRichEditView::DoPreparePrinting(CPrintInfo& info)
{
    assert(info.m_pPD);

    // Display the print dialog.
    if (info.m_pPD->DoModal(GetApp()->GetMainWnd()) != IDOK)
        return FALSE;

    // Save the printer dialog parameters so they are available after the
    // printer dialog object goes out of scope.
    PRINTDLG pd = info.m_pPD->GetParameters();
    info.SetNCopies(pd.nCopies);
    info.SetFromPage(pd.nFromPage);
    info.SetMinPage(pd.nMinPage);
    info.SetMaxPage(pd.nMaxPage);
    info.SetToPage(pd.nToPage);
    info.m_strPageDesc = L"Page %u";
    return TRUE;
}

// Prints the current document. The documents name is set by DoPrintRichView.
// If banding is supported, this should also have been set. If the printer
// uses a data type to record the print job, this should also have been set.
void CRichEditView::DoPrintView()
{
    try
    {
        CPrintInfo info;

        // Display the print dialog and create printer device context.
        if (!OnPreparePrinting(info))
            return;

        // Set the printer device context via saved info.
        CDC DC = info.m_pPD->GetPrinterDC();

        // Put the printer DC in MM_TEXT mode and compute the page m_rectDraw
        // in twips.
        DC.SetMapMode(MM_TEXT);
        int nHorizRes   = DC.GetDeviceCaps(HORZRES);
        int nVertRes    = DC.GetDeviceCaps(VERTRES);
        int nLogPixelsX = DC.GetDeviceCaps(LOGPIXELSX);
        int nLogPixelsY = DC.GetDeviceCaps(LOGPIXELSY);
        info.m_rectDraw.SetRect(0, 0, (nHorizRes * 1440) / nLogPixelsX,
            (nVertRes  * 1440) / nLogPixelsY);

        // Allocate resources and perform initializations.
        OnBeginPrinting(DC, info);

        // Set up the print job.
        DOCINFO di{};
        di.cbSize       = sizeof(DOCINFO);
        di.lpszDocName  = m_docPath; // the spooler label
        di.lpszOutput   = (m_printPath.IsEmpty() ? nullptr : m_printPath.c_str());
        di.lpszDatatype = (m_dataType.IsEmpty() ? nullptr : m_dataType.c_str());
        di.fwType       = (m_isAppBanded ?  DI_APPBANDING : 0);

        // Start the document.
        DC.StartDoc(&di);

        // Determine the printing page range.
        UINT nEndPage = info.GetToPage();
        UINT nStartPage = info.GetFromPage();

        // Assure that the starting page is within limits.
        if (nStartPage < info.GetMinPage())
            nStartPage = info.GetMinPage();
        if (nStartPage > info.GetMaxPage())
            nStartPage = info.GetMaxPage();

        // Assure that the ending page is within limits.
        if (nEndPage < info.GetMinPage())
            nEndPage = info.GetMinPage();
        if (nEndPage > info.GetMaxPage())
            nEndPage = info.GetMaxPage();

        int nStep = (nEndPage >= nStartPage) ? 1 : -1;
        nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;
        BOOL bError = FALSE;

        for (info.m_nCurPage = nStartPage; info.m_nCurPage != nEndPage;
            info.m_nCurPage += nStep)
        {
            OnPrepareDC(DC, info);

            // Check for end of print.
            if (!info.m_bContinuePrinting)
                break;

            // Attempt to start the current page.
            if (DC.StartPage() < 0)
            {
                bError = TRUE;
                break;
            }

            // We must now call OnPrepareDC again because StartPage has reset
            // the device attributes.
            OnPrepareDC(DC, info);

            assert(info.m_bContinuePrinting);

            // The page has been successfully started, so now render the page.
            OnPrint(DC, info);
            if (DC.EndPage() < 0 )
            {
                bError = TRUE;
                break;
            }
        }

        // Cleanup the document printing process and end the job.
        if (!bError)
            DC.EndDoc();
        else
            DC.AbortDoc();
        OnEndPrinting(DC, info);
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        ::MessageBox(0, L"Unable to display print dialog",
            L"Print Failed", MB_OK);
        return;
    }
}

// Print the contents of the CRichEditView control in the CView client
// area accessed by pView. Label the spooler output using the sDocPath.
void CRichEditView::DoPrintRichView(LPCWSTR sDocPath)
{
    m_docPath = sDocPath;
    DoPrintView();
}

// Calculate the vector of first characters on each page in this text rich
// edit view. Set the page count and page number limits into the info object.
void CRichEditView::GetPageBreaks(CPrintInfo& info)
{
    // Use a separate format range object for pagination.
    FORMATRANGE fr = m_fr;
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;
    long nTextScanned = 0;  // Amount of document scanned so far.

    m_firstPageChar.clear();
    m_firstPageChar.push_back(0);
    do
    {
        // Store the number of character's that fit on this page.
        nTextScanned = FormatRange(fr, FALSE);
        m_firstPageChar.push_back(nTextScanned);

        // If there is more text to format, adjust the range of characters to
        // start formatting at the first character of the next page.
        if (nTextScanned < m_textLength)
        {
            fr.chrg.cpMin = nTextScanned;
            fr.chrg.cpMax = -1;
        }
    } while (nTextScanned < m_textLength);

    // Tell the control to release cached information.
    FormatRange(fr, FALSE);

    // On exit, the m_firstPageChar vector should contain one more entry than
    // the actual number of pages.
    info.SetMinPage(1);
    info.SetFromPage(1);
    UINT maxpg = static_cast<UINT>(m_firstPageChar.size()) - 1;
    info.SetMaxPage(maxpg);
    info.SetToPage(maxpg);
}

// Initiate parameters used by the printing task. This includes setting the
// FORMATRANGE m_fr structure with the printer context, the printer page size
// and margins, the range of text to print, the beginning locations of each
// page to be printed.
void CRichEditView::OnBeginPrinting(CDC& DC, CPrintInfo& info)
{
    HDC hPrinterDC = DC.GetHDC();

    // Set up the page.
    m_fr = {};
    m_fr.hdc       = hPrinterDC;  // device to render to
    m_fr.hdcTarget = hPrinterDC;  // device to format to
    info.m_nMargin = 200; // twips
    m_fr.rcPage.left   = m_fr.rcPage.top = info.m_nMargin;
    m_fr.rcPage.right  = info.m_rectDraw.right - info.m_nMargin;
    m_fr.rcPage.bottom = info.m_rectDraw.bottom - info.m_nMargin;

    // Set up margins all around.
    m_fr.rc.left   = m_fr.rcPage.left;
    m_fr.rc.top    = m_fr.rcPage.top;
    m_fr.rc.right  = m_fr.rcPage.right;
    m_fr.rc.bottom = m_fr.rcPage.bottom;

    // Find out real size of document in characters.
    m_textLength = GetTextLengthEx(GTL_NUMCHARS);
    GetPageBreaks(info);
    info.m_bContinuePrinting = TRUE;

    // Default the range of text to print as the entire document.
    m_fr.chrg.cpMin = 0;
    m_fr.chrg.cpMax = -1;
}

// Clean up any loose ends before ending the job.
void CRichEditView::OnEndPrinting(CDC&, CPrintInfo&)
{
    // Release the cached information.
    FormatRange(m_fr, FALSE);
}

// Make preparations for printing the next page.Here, check for an
// end - of - printing condition.
void CRichEditView::OnPrepareDC(CDC&, CPrintInfo&info /* = nullptr */)
{
    if (m_fr.chrg.cpMin  >=  m_textLength)
        info.m_bContinuePrinting = FALSE;
}

// Declare a printer dialog box, set initial info settings, and get the
// printer parameters via the printer dialog.Return TRUE if a printer was
// chosen, FALSE if the dialog was cancelled.
BOOL CRichEditView::OnPreparePrinting(CPrintInfo& info)
{
    // Set up the dialog to choose the printer and printing parameters.
    // The PD_ENABLEPRINTHOOK parameter displays the classic dialog.
    MyPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC |
        PD_ENABLEPRINTHOOK);

    PrintDlg.SetBoxTitle(L"Print contents of rich edit box.");
    info.InitInfo(&PrintDlg, 1, 0xffff, 1, 0xffff, 1);
    if (!DoPreparePrinting(info))
        return FALSE;

    return TRUE;
}

// Print the current page indicated in info.
void CRichEditView::OnPrint(CDC&, CPrintInfo& info)
{
    UINT page_no = info.m_nCurPage;
    long first = m_firstPageChar[page_no - 1];
    m_fr.chrg.cpMin = first;
    m_fr.chrg.cpMax = -1;

    // Print the a page. The return value should be the index of the first
    // character on the next page. Using TRUE for the wparam parameter
    // causes the text to be printed.
    FormatRange(m_fr, TRUE);
    DisplayBand(m_fr.rc);
}

