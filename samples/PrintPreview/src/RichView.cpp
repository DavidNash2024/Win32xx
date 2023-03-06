/////////////////////////////
// RichView.cpp
//

#include "stdafx.h"
#include "RichView.h"

#ifndef WM_DPICHANGED_BEFOREPARENT
  #define WM_DPICHANGED_BEFOREPARENT      0x02E2
#endif

// Required for Dev-C++
#ifndef IMF_AUTOFONT
  #define IMF_AUTOFONT          0x0002
#endif

/////////////////////////////////
// CRichView function definitions
//

// Constructor.
CRichView::CRichView()
{
}

// Destructor.
CRichView::~CRichView()
{
}

// Calculates the character position of the page breaks, and returns
// the number of pages.
UINT CRichView::CollatePages()
{
    CPrintDialog printDlg;
    CDC printerDC = printDlg.GetPrinterDC();

    FORMATRANGE fr;
    fr.hdcTarget = printerDC;
    fr.hdc = printerDC;
    fr.rcPage = GetPageRect();
    fr.rc = GetPrintRect();
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;

    // Find out real size of document in characters.
    LONG textLength;   // Length of document.
    textLength = GetTextLengthEx(GTL_NUMCHARS);

    // Calculate the page breaks
    LONG lastChar;  // The index of the last char which fits on the page.
    m_pageBreaks.clear();
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

    // Return the number of pages.
    return static_cast<UINT>(m_pageBreaks.size());
}

// Choose the printer and start the print job.
void CRichView::DoPrint(LPCTSTR docName)
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
CRect CRichView::GetPageRect()
{
    CRect rcPage;

    // Get the device context of the default or currently chosen printer.
    CPrintDialog printDlg;
    CDC dcPrinter = printDlg.GetPrinterDC();

    // Get the printer page specifications.
    int horizRes = dcPrinter.GetDeviceCaps(HORZRES);        // in pixels
    int vertRes = dcPrinter.GetDeviceCaps(VERTRES);         // in pixels
    int logPixelsX = dcPrinter.GetDeviceCaps(LOGPIXELSX);   // in pixels per logical inch
    int logPixelsY = dcPrinter.GetDeviceCaps(LOGPIXELSY);   // in pixels per logical inch

    int tpi = 1440;     // twips per inch

    rcPage.right = (horizRes / logPixelsX) * tpi;
    rcPage.bottom = (vertRes / logPixelsY) * tpi;

    return rcPage;
}

// Returns the print area within the page. Units are measured in twips.
CRect CRichView::GetPrintRect()
{
    int margin = 200;

    CRect rcPage = GetPageRect();
    CRect rcPrintArea;

    if (!rcPage.IsRectEmpty())
    {
        rcPrintArea.left = rcPage.left + margin;
        rcPrintArea.top = rcPage.top + margin;
        rcPrintArea.right = rcPage.right - margin;
        rcPrintArea.bottom = rcPage.bottom - margin;
    }

    return rcPrintArea;
}

// Called when RichView control is created.
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

// Set the CREATESTRUCT parameters before the window is created.
void CRichView::PreCreate(CREATESTRUCT& cs)
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
                WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;

    cs.dwExStyle = WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES;
}

// Prints the specified page to specified dc.
// Called by CPrintPreview, and also used for printing.
void CRichView::PrintPage(CDC& dc, UINT page)
{
    CPrintDialog printDlg;
    CDC printerDC = printDlg.GetPrinterDC();

    // Assign values to the FORMATRANGE struct
    FORMATRANGE fr;
    ZeroMemory(&fr, sizeof(fr));
    fr.hdcTarget = printerDC;
    fr.hdc = dc;
    fr.rcPage = GetPageRect();
    fr.rc = GetPrintRect();
    fr.chrg.cpMin = (page > 0) ? m_pageBreaks[page - 1] : 0;
    fr.chrg.cpMax = m_pageBreaks[page];

    // Display text from the richedit control on the memory dc.
    FormatRange(fr, TRUE);
    DisplayBand(GetPrintRect());

    // Tell the control to release the cached information.
    FormatRange();
}

// Print the document without bringing up a print dialog.
// docName - specifies the document name for the print job.
void CRichView::QuickPrint(LPCTSTR docName)
{
    // Acquire the currently selected printer and page settings
    CPrintDialog printDlg;
    CDC printerDC = printDlg.GetPrinterDC();

    // Assign values to the FORMATRANGE struct
    FORMATRANGE fr;
    ZeroMemory(&fr, sizeof(fr));
    fr.hdc = printerDC;
    fr.hdcTarget = printerDC;

    fr.rcPage = GetPageRect();
    fr.rc = GetPrintRect();

    // Default the range of text to print as the entire document.
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;

    // Start print job.
    DOCINFO di;
    ZeroMemory(&di, sizeof(di));
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = docName;
    di.lpszOutput = NULL;   // Do not print to file.
    printerDC.StartDoc(&di);

    UINT maxPages = CollatePages();
    for (UINT page = 0; page < maxPages; ++page)
    {
        // Start the page.
        printerDC.StartPage();

        // Print the page.
        PrintPage(printerDC, page);

        // End the page.
        printerDC.EndPage();
    }

    // End the print job
    printerDC.EndDoc();
}

// Set a default font.
void CRichView::SetFontDefaults()
{
    // Set font to Courier New, size 10.
    CHARFORMAT cf;
    ZeroMemory(&cf, sizeof(cf));
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_SIZE | CFM_FACE;
    StrCopy(cf.szFaceName, _T("Courier New"), 32);
    cf.yHeight = 204;
    SetDefaultCharFormat(cf);

    // Prevent Unicode characters from changing the font
    LRESULT result = SendMessage(EM_GETLANGOPTIONS, 0, 0);
    result &= ~IMF_AUTOFONT;
    SendMessage(EM_SETLANGOPTIONS, 0, result);
}

