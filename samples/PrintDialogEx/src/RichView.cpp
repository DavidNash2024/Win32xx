/////////////////////////////
// RichView.cpp
//

#include "stdafx.h"
#include "RichView.h"


/////////////////////////////////
// CRichView function definitions
//

// Constructor.
CRichView::CRichView()
{
    ZeroMemory(&m_pageRanges, sizeof(m_pageRanges));
}

// Destructor.
CRichView::~CRichView()
{
}

// Calculates the character position of the page breaks, and returns
// the number of pages.
UINT CRichView::CollatePages(const CDC& printerDC)
{
    if (printerDC.GetHDC() == 0)
        return 0;

    FORMATRANGE fr;
    fr.hdcTarget = printerDC;
    fr.hdc = printerDC;
    fr.rcPage = GetPageRect(printerDC);
    fr.rc = GetPrintRect(printerDC);
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

    // return the number of pages.
    return static_cast<UINT>(m_pageBreaks.size());
}

// Reset the print options to default.
void CRichView::ClearPrintOptions()
{
    PRINTDLGEX pdex;
    ZeroMemory(&pdex, sizeof(pdex));
    pdex.nStartPage = START_PAGE_GENERAL;
    m_printDialog.SetParameters(pdex);
}

// Set the print options. This function can throw an exception.
void CRichView::SetPrintOptions()
{
    PRINTDLGEX pdex = m_printDialog.GetParameters();

    // Set these if not already set.
    if (pdex.nMaxPageRanges == 0)
    {
        CDC printerDC = m_printDialog.GetPrinterDC();
        pdex.nMaxPageRanges = sizeof(m_pageRanges) / sizeof(PRINTPAGERANGE);
        pdex.lpPageRanges = m_pageRanges;
        pdex.nMinPage = 1;
        pdex.nCopies = 1;
        pdex.nMaxPage = CollatePages(printerDC);
        pdex.nStartPage = START_PAGE_GENERAL;

        // These flags specify the following:
        // PD_ALLPAGES      - The All radio button is initially selected.
        // PD_NOCURRENTPAGE - Disables the Current Page radio button.
        // PD_NOSELECTION   - Disables the Selection radio button.
        pdex.Flags = PD_ALLPAGES | PD_NOCURRENTPAGE | PD_NOSELECTION;
        m_printDialog.SetParameters(pdex);
    }
}

// Choose the printer and print the document.
void CRichView::DoPrint(LPCTSTR docName)
{
    SetPrintOptions();
    INT_PTR result = m_printDialog.DoModal(*this);
    if (result == PD_RESULT_PRINT)
    {
        // Acquire the currently selected printer and page settings
        CDC printerDC = m_printDialog.GetPrinterDC();

        // Calculate the pages to print.
        std::vector<UINT> pages = SetPagesToPrint(printerDC);

        // Assign values to the FORMATRANGE struct
        FORMATRANGE fr;
        ZeroMemory(&fr, sizeof(fr));
        fr.hdc = printerDC;
        fr.hdcTarget = printerDC;
        fr.rcPage = GetPageRect(printerDC);
        fr.rc = GetPrintRect(printerDC);

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

        std::vector<UINT>::iterator i;
        for (i = pages.begin(); i < pages.end(); ++i)
        {
            // Start the page.
            printerDC.StartPage();

            // Print the page.
            PrintPage(printerDC, *i -1);

            // End the page/
            printerDC.EndPage();
        }

        // End the print job
        printerDC.EndDoc();
    }
}

// Returns a CRect of the entire printable area. Units are measured in twips.
CRect CRichView::GetPageRect(const CDC& printerDC)
{
    CRect rcPage;

    // Get the printer page specifications
    int horizRes = printerDC.GetDeviceCaps(HORZRES);        // in pixels
    int vertRes = printerDC.GetDeviceCaps(VERTRES);         // in pixels
    int logPixelsX = printerDC.GetDeviceCaps(LOGPIXELSX);   // in pixels per logical inch
    int logPixelsY = printerDC.GetDeviceCaps(LOGPIXELSY);   // in pixels per logical inch

    int tpi = 1440;     // twips per inch

    rcPage.right = (horizRes / logPixelsX) * tpi;
    rcPage.bottom = (vertRes / logPixelsY) * tpi;

    return rcPage;
}

// Returns the print area within the page. Units are measured in twips.
CRect CRichView::GetPrintRect(const CDC& printerDC)
{
    int margin = 200;

    CRect rcPage = GetPageRect(printerDC);
    CRect rcPrintArea;

    if (!rcPage.IsRectEmpty())
    {
        rcPrintArea.left = rcPage.left + margin;
        rcPrintArea.top = rcPage.top + margin;
        rcPrintArea.right = rcPage.right - margin;
        rcPrintArea.bottom = rcPage.bottom - 3*margin;  // Provides extra space for footer.
    }

    return rcPrintArea;
}

// Called when the window handle (HWND) is attached to CRichView.
void CRichView::OnAttach()
{
    //increase the text limit of the rich edit window
    LimitText(-1);

    //Determine which messages will be passed to the parent
    DWORD mask = ENM_KEYEVENTS | ENM_DROPFILES;
    SetEventMask(mask);

    SetFontDefaults();
}

LRESULT CRichView::OnSettingChange(UINT, WPARAM, LPARAM)
{
    try
    {
        // Create the printer's DC from the print dialog's current values.
        CDC printerDC;
        printerDC.CreateDC(L"winspool",
            m_printDialog.GetCurrentPrinterName(),
            m_printDialog.GetCurrentPortName(),
            m_printDialog.GetCurrentDevMode());

        // Recalculate the document's page limit.
        PRINTDLGEX pdex = m_printDialog.GetParameters();
        pdex.nMaxPage = CollatePages(printerDC);
        m_printDialog.SetParameters(pdex);
    }
    catch (const CException&)
    {
        // CreateDC throw a CResourceException on failure.
        Trace("CRichView::OnSettingChange Failed to create the current printer DC.\n");
    }

    return 0;
}

// Sets the CREATESTRUCT parameters before the window is created.
void CRichView::PreCreate(CREATESTRUCT& cs)
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
                WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;

    cs.dwExStyle = WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES;
}

// Called by PrintPage to display the page number at the bottom
// of the printed page.
void CRichView::PrintFooter(CDC& dc, UINT page)
{
    int cx = dc.GetDeviceCaps(HORZRES);         // in pixels
    int cy = dc.GetDeviceCaps(VERTRES);         // in pixels
    dc.CreatePointFont(400, L"Microsoft Sans Serif");
    CString footer("Page ");
    footer << page + 1;
    CSize textSize = dc.GetTextExtentPoint32(footer);

    int x = (cx - textSize.cx) / 2;
    int y = cy - 2 * textSize.cy;
    dc.TextOut(x, y, footer);
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
    fr.rcPage = GetPageRect(printerDC);
    fr.rc = GetPrintRect(printerDC);
    fr.chrg.cpMin = (page > 0) ? m_pageBreaks[page - 1] : 0;
    fr.chrg.cpMax = m_pageBreaks[page];

    // Display text from the richedit control on the memory dc
    FormatRange(fr, TRUE);
    DisplayBand(GetPrintRect(printerDC));

    // Tell the control to release the cached information.
    FormatRange();

    PrintFooter(dc, page);
}

// Print the document without bringing up a print dialog.
// docName - specifies the document name for the print job.
void CRichView::QuickPrint(LPCTSTR docName)
{
    // Acquire the currently selected printer and page settings
    CDC printerDC = m_printDialog.GetPrinterDC();

    // Assign values to the FORMATRANGE struct
    FORMATRANGE fr;
    ZeroMemory(&fr, sizeof(fr));
    fr.hdc = printerDC;
    fr.hdcTarget = printerDC;

    fr.rcPage = GetPageRect(printerDC);
    fr.rc = GetPrintRect(printerDC);

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

    UINT maxPages = CollatePages(printerDC);

    for (UINT page = 0; page < maxPages; ++page)
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
    //Set font
    m_font.CreatePointFont(100, _T("Courier New"));
    SetFont(m_font, FALSE);

// Required for Dev-C++
#ifndef IMF_AUTOFONT
  #define IMF_AUTOFONT          0x0002
#endif

    // Prevent Unicode characters from changing the font
    LRESULT result = SendMessage(EM_GETLANGOPTIONS, 0, 0);
    result &= ~IMF_AUTOFONT;
    SendMessage(EM_SETLANGOPTIONS, 0, result);
}

std::vector<UINT> CRichView::SetPagesToPrint(const CDC& printerDC)
{
    std::vector<UINT> pages;   // Vector of pages to print.

    PRINTDLGEX pdex = m_printDialog.GetParameters();
    BOOL isPages    = m_printDialog.PrintRange();     // Pages radio button selected.
    BOOL isCollated = m_printDialog.PrintCollate();   // Collated button selected.
    int copies      = m_printDialog.GetCopies();
    UINT maxPage     = CollatePages(printerDC);
    UINT minPage     = 1;

    if (isPages)
    {
        // Print page ranges, not the entire document.
        LPPRINTPAGERANGE rangeArray = pdex.lpPageRanges;
        UINT ranges = pdex.nPageRanges;

        // Loop for multiple copies, collated.
        for (int count1 = 0; count1 < copies; count1++)
        {
            // Loop for multiple ranges.
            for (size_t i = 0; i < ranges; i++)
            {
                UINT fromPage = rangeArray[i].nFromPage;
                UINT toPage = rangeArray[i].nToPage;
                if (fromPage <= toPage)
                {
                    fromPage = MAX(minPage, fromPage);
                    toPage   = MIN(maxPage, toPage);

                    // Loop for multiple pages
                    for (UINT j = fromPage; j <= toPage; j++)
                    {
                        // Loop for multiple copies, not collated.
                        for (int count2 = 0; count2 < copies; count2++)
                        {
                            pages.push_back(j);
                            if (isCollated) break;  // break out of loop if collated.
                        }
                    }
                }
                else
                {
                    fromPage = MIN(maxPage, fromPage);
                    toPage   = MAX(minPage, toPage);

                    // Loop for multiple pages in reverse order.
                    for (UINT j = fromPage; j >= toPage; j--)
                    {
                        // For multiple copies, not collated.
                        for (int count2 = 0; count2 < copies; count2++)
                        {
                            pages.push_back(j);
                            if (isCollated) break;  // break out of loop if collated.
                        }
                    }
                }
            }

            if (!isCollated) break;  // break out of loop if not collated.
        }
    }
    else
    {
        // Print the entire document.

        // For multiple copies, collated.
        for (int count1 = 0; count1 < copies; count1++)
        {
            for (UINT i = minPage; i <= maxPage; i++)
            {
                // For multiple copies, not collated.
                for (int count2 = 0; count2 < copies; count2++)
                {
                    pages.push_back(i);
                    if (isCollated) break;
                }
            }

            if (!isCollated) break;
        }
    }

    return pages;
}

LRESULT CRichView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        if (msg == UWM_SETTINGCHANGE) return OnSettingChange(msg, wparam, lparam);

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}