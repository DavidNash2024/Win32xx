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
UINT CRichView::CollatePages()
{
    CPrintDialogEx printDlg;
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

    // Add the final bage break.
    m_pageBreaks.push_back(-1);

    // return the number of pages.
    return static_cast<UINT>(m_pageBreaks.size());
}

void CRichView::ClearPrintOptions()
{
    PRINTDLGEX pdex;
    ZeroMemory(&pdex, sizeof(pdex));
    pdex.nStartPage = START_PAGE_GENERAL;
    m_printDialog.SetParameters(pdex);
}

void CRichView::SetPrintOptions()
{
    PRINTDLGEX pdex = m_printDialog.GetParameters();

    // Set these as required.
    pdex.nMaxPage = CollatePages();
    if (pdex.nMaxPageRanges == 0)
    {
        pdex.nMaxPageRanges = sizeof(m_pageRanges) / sizeof(PRINTPAGERANGE);
        pdex.lpPageRanges = m_pageRanges;
        pdex.nMinPage = 1;
        pdex.nCopies = 1;
        pdex.nStartPage = START_PAGE_GENERAL;

        // These flags specify the following:
        // PD_ALLPAGES      - The All radio button is initially selected.
        // PD_NOCURRENTPAGE - Disables the Current Page radio button.
        // PD_NOSELECTION   - Disables the Selection radio button.
        pdex.Flags = PD_ALLPAGES | PD_NOCURRENTPAGE | PD_NOSELECTION;
    }

    m_printDialog.SetParameters(pdex);
}

std::vector<int> CRichView::SetPagesToPrint()
{
    PRINTDLGEX pdex = m_printDialog.GetParameters();
    std::vector<int> pages;
    int copies = pdex.nCopies;
    BOOL isCollated = pdex.Flags & PD_COLLATE;
    int ranges = pdex.nPageRanges;
    if (ranges > 0)
    {
        // Print the selected page ranges.
        LPPRINTPAGERANGE rangeArray = pdex.lpPageRanges;
        for (int count1 = 0; count1 < copies; count1++)
        {
            // For multiple copies, collated.
            for (int i = 0; i < ranges; i++)
            {
                int fromPage = rangeArray[i].nFromPage;
                int toPage = rangeArray[i].nToPage;
                if (fromPage <= toPage)
                {
                    for (int j = fromPage; j <= toPage; j++)
                    {
                        // For multiple copies, not collated.
                        for (int count2 = 0; count2 < copies; count2++)
                        {
                            pages.push_back(j);
                            if (isCollated) break;
                        }
                    }
                }
                else
                {
                    // The dialog permits a page range in reverse order.
                    for (int j = fromPage; j >= toPage; j--)
                    {
                        // For multiple copies, not collated.
                        for (int count2 = 0; count2 < copies; count2++)
                        {
                            pages.push_back(j);
                            if (isCollated) break;
                        }
                    }
                }
            }

            if (!isCollated) break;
        }
    }
    else
    {
        // Print all the pages.
        int minPage = pdex.nMinPage;
        int maxPage = pdex.nMaxPage;

        // For multiple copies, collated.
        for (int count1 = 0; count1 < copies; count1++)
        {
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


// Choose the printer and print the document.
void CRichView::DoPrint(LPCTSTR docName)
{
    try
    {
        SetPrintOptions();
        INT_PTR result = m_printDialog.DoModal(*this);
        if (result == PD_RESULT_PRINT)
        {
            // Calculate the pages to print.
            std::vector<int> pages = SetPagesToPrint();

            // Acquire the currently selected printer and page settings
            CDC printerDC = m_printDialog.GetPrinterDC();
            if (printerDC.GetHDC() == 0)
                throw CResourceException(_T("No printer available"));

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

            std::vector<int>::iterator i;
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

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Failed"), MB_ICONWARNING);
    }

}

// Returns a CRect of the entire printable area. Units are measured in twips.
CRect CRichView::GetPageRect()
{
    CRect rcPage;

    // Get the device context of the default or currently chosen printer
    CPrintDialogEx printDlg;
    CDC dcPrinter = printDlg.GetPrinterDC();

    // Get the printer page specifications
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

// Sets the CREATESTRUCT parameters before the window is created.
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

    // Display text from the richedit control on the memory dc
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
    if (printerDC.GetHDC() == 0)
        throw CResourceException(_T("No printer available"));

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

