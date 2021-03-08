/* (06-Sep-2016) [Tab/Indent: 8/8][Line/Box: 80/74]            (RichView.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CRichView class using the
    Win32++ Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "RichView.h"
#include "PrintUtil.h"

/*============================================================================*/
    void CRichView::
OnAttach()                                                                  /*

    Called by the framework to initialize the rich edit view.
*-----------------------------------------------------------------------------*/
{
      //increase the text limit of the rich edit window
    LimitText(-1);
      //Determine which messages will be passed to the parent
    DWORD mask = ENM_KEYEVENTS | ENM_DROPFILES;
    SetEventMask(mask);
    SetFontDefaults();
}

/*============================================================================*/
    BOOL CRichView::
GetNewFont() const                                                          /*

    Bring up the font choice dialog and choose a new font.
*-----------------------------------------------------------------------------*/
{     // Retrieve the current character format
    CHARFORMAT cf;
    ZeroMemory(&cf, sizeof(cf));
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_COLOR | CFM_FACE | CFM_EFFECTS;
    GetDefaultCharFormat(cf);
      // Display the Choose Font dialog
    CFontDialog dlg(cf, CF_SCREENFONTS | CF_EFFECTS);
    if (dlg.DoModal(*this) == IDOK)
    {
          // Set the Font
        cf = dlg.GetCharFormat();
        SetDefaultCharFormat(cf);
    }
    return TRUE;
}

/*============================================================================*/
    UINT CRichView::
GetPageBreaks(CDC& dcPrinter)                                               /*

    Determine and record the page break locations and return the number
    of pages in the document.
*-----------------------------------------------------------------------------*/
{
      // Get the device context of the default or currently chosen printer
    dcPrinter.SetMapMode(MM_TEXT);
      // set up printer to receive pages: specific accommodation of the
      // Rich Edit document
    FORMATRANGE fr;
    fr.hdcTarget  = dcPrinter;  // format for this
    fr.hdc        = dcPrinter;  // render to this
    fr.rcPage     = GetPageRect(dcPrinter);
    fr.rc         = GetPrintRect(dcPrinter);
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;
      // Find out real size of document in characters.
    LONG textLength;   // Length of document, in characters
    textLength = GetTextLengthEx(GTL_NUMCHARS);
      // Calculate the page breaks
    LONG textPrinted;  // number of charcters printed so far
    m_pageBreaks.clear();
    do
    {
        textPrinted = FormatRange(fr, FALSE);
        if (textPrinted < textLength)
        {
            fr.chrg.cpMin = textPrinted; // next page begins here
            fr.chrg.cpMax = -1;
            m_pageBreaks.push_back(textPrinted);
        }
    } while (textPrinted < textLength);
      // Clear the cache
    FormatRange();
    m_pageBreaks.push_back(-1);
      // set the number of pages
    return static_cast<UINT>(m_pageBreaks.size());
}

/*============================================================================*/
    void CRichView::
PreCreate(CREATESTRUCT& cs)                                                 /*

    Set the rich view regular and extended styles.
*-----------------------------------------------------------------------------*/
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
        WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
    cs.dwExStyle = WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES;
}

/*============================================================================*/
    void CRichView::
PrintDC(UINT page, CDC& dcPrinter, CDC& dcDevice)                          /*

    Render the RichEdit document page formatted for the dcPrinter to the
    given dcDevice.
*-----------------------------------------------------------------------------*/
{
    FORMATRANGE fr;
    ZeroMemory(&fr, sizeof(fr));
    fr.hdcTarget  = dcPrinter;  // format for this
    fr.hdc        = dcDevice;   // render to this
    fr.rcPage     = GetPageRect(dcPrinter);
    fr.rc         = GetPrintRect(dcPrinter);
    fr.chrg.cpMin = (page > 0) ? m_pageBreaks[page - 1] : 0;
    fr.chrg.cpMax = m_pageBreaks[page];
      // display text from the RichEdit control on the memory dc
    FormatRange(fr, TRUE);
      // tell the RichView control to release its cached information.
    FormatRange();
}

/*============================================================================*/
    void CRichView::
PrintPages(CPrintDialog& printDlg)                                          /*

    Print the document without bringing up the print dialog box using the
    given printDlg object parameters.
*-----------------------------------------------------------------------------*/
{
      // get the printer's device context
    CDC dcPrinter = printDlg.GetPrinterDC();
      // assign values to the rich edit control FORMATRANGE struct
    FORMATRANGE fr;
    ZeroMemory(&fr, sizeof(fr));
    fr.hdc       = dcPrinter;               // render to this device
    fr.hdcTarget = dcPrinter;               // device to format for
    fr.rcPage    = GetPageRect(dcPrinter);  // entire area of device, twips
    fr.rc        = GetPrintRect(dcPrinter); // range of text to format
      // set the range of text to print as the entire document: defaults
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;
      // start the print job
    DOCINFO di;
    ZeroMemory(&di, sizeof(di));
    di.cbSize      = sizeof(DOCINFO);
    di.lpszDocName = m_docPath;    // name of document to print
    di.lpszOutput  = NULL;      // NULL here means output to fr.hdc
    dcPrinter.StartDoc(&di);    // start the printing
      // get the length of document to print
    LONG textLength = GetTextLengthEx(GTL_NUMCHARS);
    LONG textPrinted;  // amount of document printed so far
    do
    {
          // start at the first page
        dcPrinter.StartPage();
          // Print as much text as will fit on a page. The return
          // value is the index of the first character on the next
          // page.  Using TRUE for the wparam parameter causes the
          // text to be  printed.
        textPrinted = FormatRange(fr, TRUE);
        DisplayBand(fr.rc);
          // indicate printing this page is complete
        dcPrinter.EndPage();
          // If there is more text to print, adjust the range of
          // characters for StartPage() to begin printing at the first
          // character of the next page, for the length default of
          // characters.
        if (textPrinted < textLength)
        {
            fr.chrg.cpMin = textPrinted;
            fr.chrg.cpMax = -1;
        }
    } while (textPrinted < textLength); // while there is more to print.
      // tell the control to release its cached information
    FormatRange();
      // end the print job
    dcPrinter.EndDoc();
}

/*============================================================================*/
    BOOL CRichView::
ReadFile(LPCTSTR filePath)                                                  /*

    Open the filePath file as a rich edit view stream for display in
    the main window.
*-----------------------------------------------------------------------------*/
{
    try
    {
        // Open the file for reading
        CFile file;
        file.Open(filePath, OPEN_EXISTING);
        SetDocName(filePath);
        EDITSTREAM es;
        es.dwCookie = (DWORD_PTR) file.GetHandle();
        es.pfnCallback = (EDITSTREAMCALLBACK) RVStreamInCallback;
        StreamIn(SF_TEXT, es);
        //Clear the modified text flag
        SetModify(FALSE);
    }
    catch (const CFileException& e)
    {
        CString msg = _T("Failed to load:  ");
        msg += e.GetFilePath();
        ::MessageBox(NULL, msg, AtoT(e.what()), MB_ICONWARNING);
        return FALSE;
    }
    return TRUE;
}

/*============================================================================*/
    void CRichView::
SetFontDefaults()                                                           /*

    If the displayfont has not been set, then set it to a default Courier New
    16-pixel-height font.
*-----------------------------------------------------------------------------*/
{
      //Set font
    if (m_font.GetHandle() == 0)
        m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            FF_MODERN, _T("Courier New"));
    SetFont(m_font, FALSE);

// Required for Dev-C++
#ifndef IMF_AUTOFONT
  #define IMF_AUTOFONT          0x0002
#endif

      // Prevent Unicode characters from changing the font
    LRESULT options = SendMessage(EM_GETLANGOPTIONS, 0, 0);
    options &= ~IMF_AUTOFONT;
    SendMessage(EM_SETLANGOPTIONS, 0, options);
}

/*============================================================================*/
    void CRichView::
WordWrap(WordWrapType setting)                                              /*

    Set the rich view word wrapping to the new setting.
*-----------------------------------------------------------------------------*/
{
      // Note:  Although the CRichEdit::SetTargetDevice() function itself
      // is documented, the first two forms of it used below are not
      // described in any Microsoft documentation. In fact, the only
      // occurrences found in the Microsoft Windows Software Development
      // Kit are found in the CRichEditView::WrapChanged() method source
      // code. The CRichView::WordWrap() method derives from that source.
    switch (setting)
    {
        case WRAP_NONE:
            SetTargetDevice(NULL, 1);
            break;

        case WRAP_WINDOW:
            SetTargetDevice(NULL, 0);
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

/*============================================================================*/
    BOOL CRichView::
WriteFile(LPCTSTR filePath)                                                 /*

    Write the contents of the rich edit control to the given filePath.
*-----------------------------------------------------------------------------*/
{
    try
    {
          // Open the file for writing
        CFile file;
        file.Open(filePath, CREATE_ALWAYS);
          // stream the contents to the file via its handle
        EDITSTREAM es;
        es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
        es.dwError  = 0;
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(RVStreamOutCallback);
        StreamOut(SF_TEXT, es);
          //Clear the modified text flag
        SetModify(FALSE);
    }
    catch (const CFileException&)
    {
        CString str = _T("Failed to write:  ");
        str += filePath;
        ::MessageBox(NULL, str, _T("Warning"), MB_ICONWARNING);
        return FALSE;
    }
    return TRUE;
}

/*******************************************************************************

    Static streaming callback functions

*=============================================================================*/
    DWORD CALLBACK CRichView::
RVStreamInCallback(DWORD handle, LPBYTE buffer, LONG size, LONG *read)      /*

    Stream siaze bytes from the file handle into memory located by buffer.
    On termination read contains the number actually entered.
*-----------------------------------------------------------------------------*/
{
      // Required for StreamIn
    if (!size)
        return (1);

    *read = 0;
    if (!::ReadFile((HANDLE)(DWORD_PTR) handle, buffer, size, (LPDWORD)read, NULL))
        ::MessageBox(NULL, _T("ReadFile Failed"), _T(""), MB_OK);
    return 0;
}

/*============================================================================*/
    DWORD CALLBACK CRichView::
RVStreamOutCallback(DWORD handle, LPBYTE buffer, LONG size, LONG *out)      /*

    Stream size bytes of memory from buffer into the file handle. On
    termination out contains the number actually written.
*-----------------------------------------------------------------------------*/
{
      // Required for StreamOut
    if(!out)
        return (1);

    *out = 0;
    if (!::WriteFile((HANDLE)(DWORD_PTR)handle, buffer, size, (LPDWORD)out, NULL))
        ::MessageBox(NULL, _T("WriteFile Failed"), _T(""), MB_OK);
    return 0;
}
/*----------------------------------------------------------------------------*/