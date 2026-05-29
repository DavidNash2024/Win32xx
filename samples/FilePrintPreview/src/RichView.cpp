/////////////////////////////////////////
// RichView.cpp
// Authors: Robert Tausworthe, David Nash
//

#include "stdafx.h"
#include "RichView.h"
#include "PrintUtil.h"

//////////////////////////////////
// CRichView function definitions.
//

// Called by the framework to initialize the rich edit view.
void CRichView::OnAttach()
{
    // Increase the text limit of the rich edit window.
    LimitText(-1);

    // Determine which messages will be passed to the parent.
    DWORD mask = ENM_KEYEVENTS | ENM_DROPFILES;
    SetEventMask(mask);
    SetFontDefaults();
}

// Bring up the font choice dialog and choose a new font.
BOOL CRichView::GetNewFont() const
{
    // Retrieve the current character format.
    CHARFORMAT cf{};
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_COLOR | CFM_FACE | CFM_EFFECTS;
    GetDefaultCharFormat(cf);

    // Display the Choose Font dialog.
    CFontDialog dlg(cf, CF_SCREENFONTS | CF_EFFECTS);
    if (dlg.DoModal(*this) == IDOK)
    {
        // Set the font.
        cf = dlg.GetCharFormat();
        SetDefaultCharFormat(cf);
    }

    return TRUE;
}

// Determine and record the page break locations and return the number
// of pages in the document.
UINT CRichView::GetPageBreaks(CDC& dcPrinter)
{
    // Get the device context of the default or currently chosen printer.
    dcPrinter.SetMapMode(MM_TEXT);

    // Set up printer to receive pages: specific accommodation of the
    // Rich Edit document.
    FORMATRANGE fr;
    fr.hdcTarget  = dcPrinter;  // format for this
    fr.hdc        = dcPrinter;  // render to this
    fr.rcPage     = GetPageRect(dcPrinter);
    fr.rc         = GetPrintRect(dcPrinter);
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;

    // Find out real size of document in characters.
    LONG textLength = GetTextLengthEx(GTL_NUMCHARS);

    // Calculate the page breaks.
    LONG textPrinted;  // Number of characters printed so far.
    m_pageBreaks.clear();
    do
    {
        textPrinted = FormatRange(fr, FALSE);
        if (textPrinted < textLength)
        {
            fr.chrg.cpMin = textPrinted; // Next page begins here.
            fr.chrg.cpMax = -1;
            m_pageBreaks.push_back(textPrinted);
        }
    } while (textPrinted < textLength);

    // Clear the cache.
    FormatRange();
    m_pageBreaks.push_back(-1);

    // Set the number of pages.
    return static_cast<UINT>(m_pageBreaks.size());
}

// Set the rich view regular and extended styles.
void CRichView::PreCreate(CREATESTRUCT& cs)
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
        WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
    cs.dwExStyle = WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES;
}

// Render the RichEdit document page formatted for the dcPrinter to the
// given dcDevice.
void CRichView::PrintDC(UINT page, CDC& dcPrinter, CDC& dcDevice)
{
    FORMATRANGE fr{};
    fr.hdcTarget  = dcPrinter;  // format for this
    fr.hdc        = dcDevice;   // render to this
    fr.rcPage     = GetPageRect(dcPrinter);
    fr.rc         = GetPrintRect(dcPrinter);
    fr.chrg.cpMin = (page > 0) ? m_pageBreaks[page - 1] : 0;
    fr.chrg.cpMax = m_pageBreaks[page];

    // Display text from the RichEdit control on the memory dc.
    FormatRange(fr, TRUE);

    // Tell the RichView control to release its cached information.
    FormatRange();
}

// Print the document without bringing up the print dialog box using the
// given printDlg object parameters.
void CRichView::PrintPages(CPrintDialog& printDlg)
{
    // Get the printer's device context.
    CDC dcPrinter = printDlg.GetPrinterDC();

    // Assign values to the rich edit control FORMATRANGE struct.
    FORMATRANGE fr{};
    fr.hdc       = dcPrinter;               // render to this device
    fr.hdcTarget = dcPrinter;               // device to format for
    fr.rcPage    = GetPageRect(dcPrinter);  // entire area of device, twips
    fr.rc        = GetPrintRect(dcPrinter); // range of text to format

    // Set the range of text to print as the entire document.
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;

    // Start the print job.
    DOCINFO di{};
    di.cbSize      = sizeof(DOCINFO);
    di.lpszDocName = m_docPath;    // name of document to print
    di.lpszOutput  = nullptr;      // nullptr here means output to fr.hdc
    dcPrinter.StartDoc(&di);    // start the printing

    // Get the length of document to print.
    LONG textLength = GetTextLengthEx(GTL_NUMCHARS);
    LONG textPrinted;  // Amount of document printed so far.
    do
    {
        // Start at the first page.
        dcPrinter.StartPage();

        // Print as much text as will fit on a page. The return value is the
        // index of the first character on the next page. Using TRUE for the
        // wparam parameter causes the text to be  printed.
        textPrinted = FormatRange(fr, TRUE);
        DisplayBand(fr.rc);

        // Indicate printing this page is complete.
        dcPrinter.EndPage();

        // If there is more text to print, adjust the range of characters for
        // StartPage to begin printing at the first character of the next page,
        // for the length default of characters.
        if (textPrinted < textLength)
        {
            fr.chrg.cpMin = textPrinted;
            fr.chrg.cpMax = -1;
        }
    } while (textPrinted < textLength); // While there is more to print.

    // Tell the control to release its cached information.
    FormatRange();

    // End the print job.
    dcPrinter.EndDoc();
}

// Open the filePath file as a rich edit view stream for display in the main
// window.
BOOL CRichView::ReadFile(LPCWSTR filePath)
{
    try
    {
        // Open the file for reading.
        CFile file;
        file.Open(filePath, OPEN_EXISTING | CFile::modeRead);
        SetDocName(filePath);
        EDITSTREAM es;
        es.dwCookie = (DWORD_PTR) file.GetHandle();
        es.pfnCallback = (EDITSTREAMCALLBACK) RVStreamInCallback;
        StreamIn(SF_TEXT, es);

        // Clear the modified text flag.
        SetModify(FALSE);
    }
    catch (const CFileException& e)
    {
        CString msg = "Failed to load:  ";
        msg += e.GetFilePath();
        ::MessageBox(nullptr, msg, AtoW(e.what()), MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}

// If the display font has not been set, then set it to a default Courier New
// size 10 font.
void CRichView::SetFontDefaults()
{
    // Create a default font if required.
    if (m_font.GetHandle() == nullptr)
    {
        constexpr int fontSize = 10;
        constexpr int tenthsOfPoint = 10;
        m_font.CreatePointFont(fontSize * tenthsOfPoint, L"Courier New");
    }

    // Set the font.
    SetFont(m_font, FALSE);

    // Prevent Unicode characters from changing the font.
    LRESULT options = SendMessage(EM_GETLANGOPTIONS, 0, 0);
    options &= ~IMF_AUTOFONT;
    SendMessage(EM_SETLANGOPTIONS, 0, options);
}

// Set the rich view word wrapping to the new setting.
void CRichView::WordWrap(WordWrapType setting)
{
    switch (setting)
    {
        case WRAP_NONE:
            SetTargetDevice(0, 1);
            break;

        case WRAP_WINDOW:
            SetTargetDevice(0, 0);
            break;

        case WRAP_PRINTER:
        {
            CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
            CDC dcPrinter = printDlg.GetPrinterDC();
            CRect rc = GetPrintRect(dcPrinter);
            SetTargetDevice(dcPrinter, rc.Width());
            break;
        }

        default:
            break;
    }
}

// Write the contents of the rich edit control to the given filePath.
BOOL CRichView::WriteFile(LPCWSTR filePath)
{
    try
    {
        // Open the file for writing.
        CFile file;
        file.Open(filePath, CREATE_ALWAYS);

        // Stream the contents to the file via its handle.
        EDITSTREAM es;
        es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
        es.dwError  = 0;
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(RVStreamOutCallback);
        StreamOut(SF_TEXT, es);

        // Clear the modified text flag.
        SetModify(FALSE);
    }
    catch (const CFileException&)
    {
        CString str = L"Failed to write:  ";
        str += filePath;
        ::MessageBox(nullptr, str, L"Warning", MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}



// Static streaming callback functions.

// Stream size bytes from the file handle into memory located by buffer.
// On termination read contains the number actually entered.
DWORD CALLBACK CRichView::RVStreamInCallback(DWORD handle, LPBYTE buffer,
    LONG size, LONG *read)
{
    *read = 0;
    if (!::ReadFile((HANDLE)(DWORD_PTR) handle, buffer, size, (LPDWORD)read, nullptr))
        ::MessageBox(0, L"ReadFile Failed", L"", MB_OK);
    return 0;
}

// Stream size bytes of memory from buffer into the file handle.On
// termination out contains the number actually written.
DWORD CALLBACK CRichView::RVStreamOutCallback(DWORD handle, LPBYTE buffer,
    LONG size, LONG *out)
{
    *out = 0;
    if (!::WriteFile((HANDLE)(DWORD_PTR)handle, buffer, size, (LPDWORD)out, nullptr))
        ::MessageBox(nullptr, L"WriteFile Failed", L"", MB_OK);
    return 0;
}
