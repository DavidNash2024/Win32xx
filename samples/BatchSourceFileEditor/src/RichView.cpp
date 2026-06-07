/////////////////////////////
// RichView.cpp
//

#include "stdafx.h"
#include "richview.h"

//////////////////////////////////////
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

void CRichView::AppendBigWarning(LPCTSTR text)
{
    UseLargeFont(RGB(255, 0, 0));
    AppendText(text);
}
void CRichView::AppendSmallWarning(LPCTSTR text)
{
    UseSmallFont(RGB(255, 0, 0));
    AppendText(text);
}

void CRichView::AppendBigText(LPCTSTR text)
{
    UseLargeFont(RGB(0, 0, 128));
    AppendText(text);
}
void CRichView::AppendSmallText(LPCTSTR text)
{
    UseSmallFont(RGB(0, 0, 128));
    AppendText(text);
}



// Calculates the character position of the page breaks, and returns
// the number of pages.
UINT CRichView::CollatePages()
{
    try
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
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Failed to get printer", MB_ICONWARNING);
    }

    // return the number of pages.
    return static_cast<UINT>(m_pageBreaks.size());
}

// Prints the contents of the view window.
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

    try
    {
        // Get the device context of the default or currently chosen printer
        CPrintDialog printDlg;
        CDC dcPrinter = printDlg.GetPrinterDC();

        // Get the printer page specifications
        int horizRes = dcPrinter.GetDeviceCaps(HORZRES);        // in pixels
        int vertRes = dcPrinter.GetDeviceCaps(VERTRES);         // in pixels
        int logPixelsX = dcPrinter.GetDeviceCaps(LOGPIXELSX);   // in pixels per logical inch
        int logPixelsY = dcPrinter.GetDeviceCaps(LOGPIXELSY);   // in pixels per logical inch

        int tpi = 1440;     // twips per inch

        rcPage.right = (horizRes / logPixelsX) * tpi;
        rcPage.bottom = (vertRes / logPixelsY) * tpi;
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"No printer available", MB_ICONWARNING);
    }

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
    // Set rich text mode.
    SetTextMode(TM_RICHTEXT);

    // Advises the control to be per-monitor DPI aware.
    // This affects the initial font size on a second monitor with different DPI.
    SendMessage(WM_DPICHANGED_BEFOREPARENT);
}

// Sets the CREATESTRUCT parameters before the window is created.
void CRichView::PreCreate(CREATESTRUCT& cs)
{
    cs.style =  ES_MULTILINE | WS_CHILD | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
    cs.dwExStyle = WS_EX_CLIENTEDGE;
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

    // Assign values to the FORMATRANGE struct
    FORMATRANGE fr = {};
    fr.hdc = printerDC;
    fr.hdcTarget = printerDC;

    fr.rcPage = GetPageRect();
    fr.rc = GetPrintRect();

    // Default the range of text to print as the entire document.
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;

    // Start print job.
    DOCINFO di = {};
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = docName;
    di.lpszOutput = nullptr;   // Do not print to file.
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

// Selects the larger font.
void CRichView::UseLargeFont(COLORREF color)
{
    CHARFORMAT cf;
    cf.cbSize = sizeof(cf);
    cf.dwEffects = CFE_BOLD;
    cf.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_COLOR;
    int charSize = 10;
    int tppp = 20;                         // twips per printer point
    cf.yHeight = charSize * tppp;
    cf.crTextColor = color;
    size_t size = sizeof(cf.szFaceName) / sizeof(TCHAR);
    StrCopy(cf.szFaceName, L"Consolas", size);
    SetSelectionCharFormat(cf);
}

// Selects the smaller font.
void CRichView::UseSmallFont(COLORREF color)
{
    CHARFORMAT cf;
    cf.cbSize = sizeof(cf);
    cf.dwEffects = 0;                      // not bold
    cf.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_COLOR;
    int charSize = 9;
    int tppp = 20;                         // twips per printer point
    cf.yHeight = charSize * tppp;
    cf.crTextColor = color;
    size_t size = sizeof(cf.szFaceName) / sizeof(TCHAR);
    StrCopy(cf.szFaceName, L"Consolas", size);
    SetSelectionCharFormat(cf);
}
