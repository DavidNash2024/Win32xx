/////////////////////////////
// RichView.cpp
//

#include "stdafx.h"
#include "RichView.h"
#include "UserMessages.h"


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
int CRichView::CollatePages(const CDC& printerDC)
{
    if (printerDC.GetHDC() == NULL)
        return 0;

    // Find the first and last characters.
    LONG firstChar = 0;
    LONG lastChar = GetTextLengthEx(GTL_NUMCHARS);
    if (m_printDialog.IsPrintSelection())
    {
        LONG first = 0;
        LONG last = 0;
        GetSel(first, last);
        if (first != last)   // Is anything selected?
        {
            firstChar = first;
            lastChar = last;
        }
    }

    FORMATRANGE fr;
    fr.hdcTarget = printerDC;
    fr.hdc = printerDC;
    fr.rcPage = GetPageRect(printerDC);
    fr.rc = GetPrintRect(printerDC);
    fr.chrg.cpMin = firstChar;
    fr.chrg.cpMax = lastChar;

    // Calculate the page breaks
    LONG lastCharOnPage;  // The index of the last char that fits on the page.
    m_pageBreaks.clear();
    do
    {
        lastCharOnPage = FormatRange(fr, FALSE);
        fr.chrg.cpMin = lastCharOnPage;
        fr.chrg.cpMax = lastChar;
        m_pageBreaks.push_back(lastCharOnPage);   // store the page break index in the vector

    } while (lastCharOnPage < lastChar);

    // return the number of pages.
    return static_cast<int>(m_pageBreaks.size());
}

// Choose the printer and print the document.
void CRichView::DoPrint(LPCTSTR docName)
{
    // Set initial pages selection, along with to and from pages.
    SetDefaultPrintOptions();

    INT_PTR result = m_printDialog.DoModal(*this);
    if (result == IDOK)
    {
        // Acquire the currently selected printer and page settings.
        CDC printerDC = m_printDialog.GetPrinterDC();
        m_printDialog.SetFromPage(std::max(m_printDialog.GetFromPage(), 1));
        m_printDialog.SetToPage(std::min(m_printDialog.GetToPage(), CollatePages(printerDC)));
        m_printDialog.SetFromPage(std::min(m_printDialog.GetFromPage(), m_printDialog.GetToPage()));

        // Calculate the pages to print.
        std::vector<int> pages = SetPagesToPrint(printerDC);

        // Find the first and last characters.
        LONG firstChar = 0;
        LONG lastChar = GetTextLengthEx(GTL_NUMCHARS);
        if (m_printDialog.IsPrintSelection())
        {
            LONG first = 0;
            LONG last = 0;
            GetSel(first, last);
            if (first != last)   // Is anything selected?
            {
                firstChar = first;
                lastChar = last;
            }
        }

        // Assign values to the FORMATRANGE struct
        FORMATRANGE fr;
        ZeroMemory(&fr, sizeof(fr));
        fr.hdc = printerDC;
        fr.hdcTarget = printerDC;
        fr.rcPage = GetPageRect(printerDC);
        fr.rc = GetPrintRect(printerDC);
        fr.chrg.cpMin = firstChar;
        fr.chrg.cpMax = lastChar;

        // Start print job.
        DOCINFO di;
        ZeroMemory(&di, sizeof(di));
        di.cbSize = sizeof(DOCINFO);
        di.lpszDocName = docName;
        di.lpszOutput = NULL;   // Do not print to file.
        printerDC.StartDoc(&di);

        std::vector<int>::iterator i;
        for (i = pages.begin(); i != pages.end(); ++i)
        {
            // Start the page.
            printerDC.StartPage();

            // Print the page.
            PrintPage(printerDC, *i - 1);

            // End the page.
            printerDC.EndPage();
        }

        // End the print job
        printerDC.EndDoc();
    }
}

// Returns a CRect of the entire printable area. Units are measured in twips.
CRect CRichView::GetPageRect(const CDC& dc)
{
    CRect rcPage;

    // Get the printer page specifications
    int horizRes = dc.GetDeviceCaps(HORZRES);        // in pixels
    int vertRes = dc.GetDeviceCaps(VERTRES);         // in pixels
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
    CDC printerDC = m_printDialog.GetPrinterDC();

    // Find the first character.
    LONG firstChar = 0;
    if (m_printDialog.IsPrintSelection())
    {
        // Check if something is selected.
        LONG first = 0;
        LONG last = 0;
        GetSel(first, last);
        if (first != last)   // Is anything selected?
        {
            firstChar = first;
        }
    }

    // Assign values to the FORMATRANGE struct
    FORMATRANGE fr;
    ZeroMemory(&fr, sizeof(fr));
    fr.hdcTarget = printerDC;
    fr.hdc = dc;
    fr.rcPage = GetPageRect(printerDC);
    fr.rc = GetPrintRect(printerDC);
    fr.chrg.cpMin = (page > 0) ? m_pageBreaks[page - 1] : firstChar;
    fr.chrg.cpMax = m_pageBreaks[page];

    // Display text from the richedit control on the memory dc.
    FormatRange(fr, TRUE);
    DisplayBand(GetPrintRect(printerDC));

    // Tell the control to release the cached information.
    FormatRange();
}

// Print the entire document without bringing up a print dialog.
// docName - specifies the document name for the print job.
void CRichView::QuickPrint(LPCTSTR docName)
{
    // Default to printing entire document, 1 copy.
    SetDefaultPrintOptions();

    // Acquire the currently selected printer's device context.
    CDC printerDC = m_printDialog.GetPrinterDC();

    // Start print job.
    DOCINFO di;
    ZeroMemory(&di, sizeof(di));
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = docName;
    di.lpszOutput = NULL;   // Do not print to file.
    printerDC.StartDoc(&di);

    int maxPages = CollatePages(printerDC);

    for (int page = 0; page < maxPages; ++page)
    {
        // Start the page.
        printerDC.StartPage();

        // Print the page.
        PrintPage(printerDC, page);

        // End the page.
        printerDC.EndPage();
    }

    // End the print job.
    printerDC.EndDoc();
}

// Sets the initial font for the document.
void CRichView::SetFontDefaults()
{
    // Set font to Courier New, size 10.
    CHARFORMAT cf;
    ZeroMemory(&cf, sizeof(cf));
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_SIZE | CFM_FACE | CFM_EFFECTS;
    StrCopy(cf.szFaceName, _T("Courier New"), 32);
    cf.yHeight = 204;
    SetDefaultCharFormat(cf);

    // Prevent Unicode characters from changing the font.
    LRESULT result = SendMessage(EM_GETLANGOPTIONS, 0, 0);
    result &= ~IMF_AUTOFONT;
    SendMessage(EM_SETLANGOPTIONS, 0, result);
}

std::vector<int> CRichView::SetPagesToPrint(const CDC& printerDC)
{
    std::vector<int> pages;   // Vector of pages to print.

    BOOL isPages = m_printDialog.IsPrintRange();     // Pages radio button selected.
    BOOL isCollated = m_printDialog.IsCollate();     // Collated button selected.
    int copies = m_printDialog.GetCopies();
    int maxPage = CollatePages(printerDC);
    int minPage = 1;

    if (isPages)
    {
        // Loop for multiple copies, collated.
        for (int count1 = 0; count1 < copies; count1++)
        {
            int fromPage = m_printDialog.GetFromPage();
            int toPage = m_printDialog.GetToPage();
            if (fromPage <= toPage)
            {
                fromPage = std::max(minPage, fromPage);
                toPage = std::min(maxPage, toPage);

                // Loop for multiple pages.
                for (int j = fromPage; j <= toPage; j++)
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
                fromPage = std::min(maxPage, fromPage);
                toPage = std::max(minPage, toPage);

                // Loop for multiple pages in reverse order.
                for (int j = fromPage; j >= toPage; j--)
                {
                    // For multiple copies, not collated.
                    for (int count2 = 0; count2 < copies; count2++)
                    {
                        pages.push_back(j);
                        if (isCollated) break;  // break out of loop if collated.
                    }
                }
            }

            if (!isCollated) break;  // break out of loop if not collated.
        }
    }
    else
    {
        // Print the entire or selected document, multiple copies.
        for (int count1 = 0; count1 < copies; count1++)
        {
            // For multiple copies, collated.
            for (int i = minPage; i <= maxPage; i++)
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

// Sets the initial dialog options.
void CRichView::SetDefaultPrintOptions()
{
    m_printDialog.SetRadio(0);    // Set print range to All pages
    m_printDialog.SetCopies(1);
    m_printDialog.SetFromPage(1);
    CDC printerDC = m_printDialog.GetPrinterDC();
    int maxPage = CollatePages(printerDC);
    m_printDialog.SetMaxPage(maxPage);
    m_printDialog.SetToPage(maxPage);
    m_printDialog.SetCollate(1);  // Tick the collate button
}

LRESULT CRichView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_SETDEFAULTOPTIONS:
        {
            SetDefaultPrintOptions();
        }
        break;
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}
