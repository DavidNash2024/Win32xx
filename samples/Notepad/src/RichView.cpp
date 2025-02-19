/////////////////////////////
// RichView.cpp
//

#include "stdafx.h"
#include "RichView.h"


// Required for Dev-C++
#ifndef IMF_AUTOFONT
  #define IMF_AUTOFONT          0x0002
#endif


/////////////////////////////////
// CRichView function definitions
//

// Calculates the character position of the page breaks, and returns
// the number of pages.
int CRichView::CollatePages(const CDC& printerDC, int startChar, int endChar)
{
    if (endChar == -1)
        endChar = GetTextLengthEx(GTL_NUMCHARS);;

    assert(startChar <= endChar);
    m_pageBreaks.clear();

    FORMATRANGE fr;
    fr.hdcTarget = printerDC;
    fr.hdc = printerDC;
    fr.rcPage = GetPageRect(printerDC);
    fr.rc = GetPrintRect(printerDC);
    fr.chrg.cpMin = startChar;
    fr.chrg.cpMax = endChar;

    // Find out real size of document in characters.
    LONG textLength;   // Length of document.
    textLength = GetTextLengthEx(GTL_NUMCHARS);

    // Calculate the page breaks
    LONG lastChar;  // The index of the last char that fits on the page.
    do
    {
        lastChar = FormatRange(fr, FALSE);

        if (lastChar < textLength)
        {
            fr.chrg.cpMin = lastChar;
            fr.chrg.cpMax = -1;
            m_pageBreaks.push_back(lastChar);   // store the page break index in the vector
        }
    } while (lastChar < textLength);

    // Add the final page break.
    m_pageBreaks.push_back(-1);

    // return the number of pages.
    return static_cast<int>(m_pageBreaks.size());
}

// Choose the printer and print the document.
void CRichView::DoPrint(LPCWSTR docName)
{
    // Prepare the print dialog
    CPrintDialog printDlg;
    PRINTDLG pd = printDlg.GetParameters();
    pd.nCopies = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage = 0xFFFF;
    pd.nMinPage = 1;
    pd.nMaxPage = 0xFFFF;
    printDlg.SetParameters(pd);

    // Display the print dialog. Can throw if there is no printer.
    if (printDlg.DoModal(*this) == IDOK)
        QuickPrint(docName);
}

// Returns a CRect of the entire printable area. Units are measured in twips.
CRect CRichView::GetPageRect(const CDC& dc)
{
    CRect rcPage;

    // Get the printer page specifications
    int horizRes = dc.GetDeviceCaps(HORZRES);        // in pixels
    int vertRes  = dc.GetDeviceCaps(VERTRES);         // in pixels
    int logPixelsX = dc.GetDeviceCaps(LOGPIXELSX);   // in pixels per logical inch
    int logPixelsY = dc.GetDeviceCaps(LOGPIXELSY);   // in pixels per logical inch

    int tpi = 1440;     // twips per inch

    rcPage.right = (horizRes / logPixelsX) * tpi;
    rcPage.bottom = (vertRes / logPixelsY) * tpi;

    return rcPage;
}

// Returns the print area within the page. Units are measured in twips.
CRect CRichView::GetPrintRect(const CDC& dc)
{
    CRect rcPage = GetPageRect(dc);
    CRect rcPrintArea;

    // Scale the margins.
    int dpiX = dc.GetDeviceCaps(LOGPIXELSX);
    int dpiY = dc.GetDeviceCaps(LOGPIXELSY);

    if (!rcPage.IsRectEmpty())
    {
        rcPrintArea.left = rcPage.left + dpiX / 3;
        rcPrintArea.top = rcPage.top + dpiY / 3;
        rcPrintArea.right = rcPage.right - dpiX / 3;
        rcPrintArea.bottom = rcPage.bottom - dpiY / 3;
    }

    return rcPrintArea;
}

// Called when the window handle (HWND) is attached to CRichView.
void CRichView::OnAttach()
{
    // Increase the text limit of the rich edit window.
    LimitText(-1);

    // Determine which messages will be passed to the parent.
    DWORD mask = ENM_KEYEVENTS | ENM_DROPFILES;
    SetEventMask(mask);

    // Advises the control to be per-monitor DPI aware.
    // This affects the initial font size on a second monitor with different DPI.
    SendMessage(WM_DPICHANGED_BEFOREPARENT);

    SetFontDefaults();
}

// Sets the CREATESTRUCT parameters before the window is created.
void CRichView::PreCreate(CREATESTRUCT& cs)
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
                WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;

    cs.dwExStyle = WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES;
}

// Prints the specified page to specified dc.
// Called by CPrintPreview, and also used for printing.
void CRichView::PrintPage(CDC& dc, int page)
{
    CPrintDialog printDlg;
    CDC printerDC = printDlg.GetPrinterDC();

    // Assign values to the FORMATRANGE struct
    FORMATRANGE fr{};
    fr.hdcTarget = printerDC;
    fr.hdc = dc;
    fr.rcPage = GetPageRect(printerDC);
    fr.rc = GetPrintRect(printerDC);
    fr.chrg.cpMin = (page > 0) ? m_pageBreaks[page - 1] : 0;
    fr.chrg.cpMax = m_pageBreaks[page];

    // Display text from the richedit control on the memory dc
    FormatRange(fr, TRUE);
    DisplayBand(GetPrintRect(printerDC));

    // Tell the control to release the cached information.
    FormatRange();
}

// Print the document without bringing up a print dialog.
// docName - specifies the document name for the print job.
void CRichView::QuickPrint(LPCWSTR docName)
{
    // Acquire the currently selected printer and page settings
    CPrintDialog printDlg;
    CDC printerDC = printDlg.GetPrinterDC();

    // Assign values to the FORMATRANGE struct
    FORMATRANGE fr{};
    fr.hdc = printerDC;
    fr.hdcTarget = printerDC;

    fr.rcPage = GetPageRect(printerDC);
    fr.rc = GetPrintRect(printerDC);

    // Default the range of text to print as the entire document.
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;

    // Start print job.
    DOCINFO di{};
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = docName;
    di.lpszOutput = nullptr;   // Do not print to file.
    printerDC.StartDoc(&di);

    int maxPages = CollatePages(printerDC);
    for (int page = 0; page < maxPages; ++page)
    {
        // Start the page.
        printerDC.StartPage();

        // Print the page.
        PrintPage(printerDC, page);

        // End the page/
        printerDC.EndPage();
    }

    // End the print job
    printerDC.EndDoc();
}

// Sets the initial font for the document.
void CRichView::SetFontDefaults()
{
    // Set font to Courier New, size 10.
    CHARFORMAT cf{};
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_SIZE | CFM_FACE | CFM_EFFECTS;
    StrCopy(cf.szFaceName, L"Courier New", LF_FACESIZE);
    constexpr int twipsPerPoint = 20;
    constexpr int fontSize = 10;
    cf.yHeight = fontSize * twipsPerPoint;
    SetDefaultCharFormat(cf);

    // Prevent Unicode characters from changing the font.
    LRESULT result = SendMessage(EM_GETLANGOPTIONS, 0, 0);
    result &= ~IMF_AUTOFONT;
    SendMessage(EM_SETLANGOPTIONS, 0, result);
}

// Handle the window's messages.
LRESULT CRichView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}