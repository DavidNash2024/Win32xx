/* (06-Sep-2016) [Tab/Indent: 8/8][Line/Box: 80/74]          (CRichEditView.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CRichEditView class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes, Copyright (c) 2005-2016 David Nash, under permissions granted
    therein.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and to the alterations,
    additions, deletions, and other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and any associated documentation composed by
    the author, to utilize this material, free of charge and without
    restriction or limitation, subject to the following conditions:

        The above copyright notice, as well as that of David Nash
        and Win32++, together with the respective permission
        conditions shall be included in all copies or substantial
        portions of this material so copied, modified, merged,
        published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.


    Special Conventions:

    Acknowledgement:
        The author would like to thank and acknowledge the advice,
        critical review, insight, and assistance provided by David Nash
        in the development of this work.

    Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Implementation of the CRichEditView class

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

#ifndef CFM_BACKCOLOR
#define CFM_BACKCOLOR       0x04000000
#endif

/*============================================================================*/
    CRichEditView::
CRichEditView()                                                         /*

*-----------------------------------------------------------------------------*/
{
    m_sDocPath.Empty();
    m_sPrintPath.Empty();
    m_sDataType.Empty();
    m_bAppBanding = FALSE;

}

/*============================================================================*/
    CRichEditView::
~CRichEditView()                                                        /*

*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    void CRichEditView::
Clean()                                 /*

    Clear the control of all text.
*-----------------------------------------------------------------------------*/
{
    SetWindowText(_T(""));
}

/*============================================================================*/
    BOOL CRichEditView::
IsSelected()                                /*

    Return TRUE if text is currently selected.
*-----------------------------------------------------------------------------*/
{
    CHARRANGE r;
    GetSel(r);
    return (r.cpMin != r.cpMax);
}


/*============================================================================*/
    void CRichEditView::
OnAttach()                                              /*

*-----------------------------------------------------------------------------*/
{
      // increase the text limit of the rich edit window: here, (DWORD)-1,
      // or the maximum DWORD value.
    LimitText(-1);

      //Determine which messages will be passed to the parent
    DWORD dwMask = ENM_KEYEVENTS | ENM_DROPFILES | ENM_MOUSEEVENTS;
    SetEventMask(dwMask);
}

/*============================================================================*/
    void CRichEditView::
SetColors(COLORREF txfg, COLORREF txbg, COLORREF bg)                  /*

    Set the rich edit control text foreground and background colors and the
    control background color.  This is needed only when the colors change
    (not like other controls).
*-----------------------------------------------------------------------------*/
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

/*============================================================================*/
    void CRichEditView::
SetFont(HFONT hFont, BOOL bRedraw) const                                /*

*-----------------------------------------------------------------------------*/
{
    CRichEdit::SetFont(hFont, bRedraw);

  // Required for Dev-C++
#ifndef IMF_AUTOFONT
#define IMF_AUTOFONT            0x0002
#endif

      // Prevent Unicode characters from changing the font: first, get the
      // rich edit option settings
    LRESULT lres = SendMessage(EM_GETLANGOPTIONS, 0, 0);
      // do not automatically change fonts when the user explicitly changes
      // to a different keyboard layout.
    lres &= ~IMF_AUTOFONT;
    SendMessage(EM_SETLANGOPTIONS, 0, lres);
}

/*============================================================================*/
    void CRichEditView::
SetWrapping(int wrap)                                                  /*

    Set the rich edit view window to use word wrapping (wrap is TRUE) or
    not use word wrapping (wrap is FALSE), or to use word wrapping to a
    given length (wrap = length > 1).
*-----------------------------------------------------------------------------*/
{
      // This feature is not documented by Microsoft. An internet search
      // reveals that the wrap values is interpreted as follows:
      // 0  turns word wrap on based on the window width
      // 1  turns word wrap off completely
      // >1 turns word wrap on for a line width equal to the wrap value
    SetTargetDevice(NULL, (wrap == 0 ? TRUE : FALSE));
}

/*============================================================================*/
    BOOL CRichEditView::
StreamInFile(const CFile& file, BOOL mode)                  /*

    Read the text-only contents of the opened file into this rich edit
    control. Set the modified state FALSE and return TRUE on success. Return
    FALSE otherwise. The file must have been successfully opened in the
    OPEN_EXISTING mode.
*-----------------------------------------------------------------------------*/
{
    try
    {
        UINT format = SF_TEXT;
        if (mode == TRUE)
            format |= SF_UNICODE;
        EDITSTREAM es;
        es.dwCookie =  (DWORD_PTR) file.GetHandle();
        es.pfnCallback = (EDITSTREAMCALLBACK) StreamInCallback;
        StreamIn(format, es);
    }

    catch (const CFileException& e)
    {
        CString str = _T("Failed to load:  ");
        str += e.GetFilePath();
        ::MessageBox(NULL, str, AtoT(e.what()), MB_ICONWARNING);
        return FALSE;
    }

    SetModify(FALSE);
    return TRUE;
}

/*============================================================================*/
    BOOL CRichEditView::
StreamOutFile(const CFile& file, BOOL mode)                             /*

    Write the text-only contents of this rich edit control into the opened
    file. Set the modified state FALSE and return TRUE on success. Return
    FALSE otherwise. The file must have been successfully opened in the
    CREATE_ALWAYS mode.
*-----------------------------------------------------------------------------*/
{
    try
    {
        UINT format = SF_TEXT;
        if (mode == TRUE)
            format |= SF_UNICODE;
        EDITSTREAM es;
        es.dwCookie =  (DWORD_PTR) file.GetHandle();
        es.dwError = 0;
        es.pfnCallback = (EDITSTREAMCALLBACK) StreamOutCallback;
        StreamOut(format, es);

        //Clear the modified text flag
    }

    catch (const CFileException&)
    {
        CString str = _T("Failed to write:\n    ");
        str += file.GetFilePath();
        ::MessageBox(NULL, str, _T("Warning"), MB_ICONWARNING);
        return FALSE;
    }

    SetModify(FALSE);
    return TRUE;
}

/*******************************************************************************

    Static Callback Functions

*=============================================================================*/
    DWORD CALLBACK CRichEditView::
StreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)   /*

    Replace the contents of the rich edit view window with data from an
    input stream. The dwCookie is an app-defined value that directs the
    deposition of cb bytes of data into the pbBuff. The pcb points to the
    location receiving the number of bytes actually transferred in. Returns
    zero on success, or nonzero if cb is zero, indicating completion.
    Throws an exception on gilr error. In this usage, dwCookie contains the
    handle of an open file.
*-----------------------------------------------------------------------------*/
{
    // Required for StreamIn termination
    if (!cb)
        return (1);

    *pcb = 0;
    if (!::ReadFile((HANDLE)(DWORD_PTR) dwCookie, pbBuff, cb, (LPDWORD)pcb,
        NULL))
        ::MessageBox(NULL, _T("StreamInFile Failed"), _T(""), MB_OK);

    return 0;
}

/*============================================================================*/
    DWORD CALLBACK CRichEditView::
StreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)  /*

    Tansfer data out of the rich edit view window. The dwCookie is an
    app-defined value that directs the transfer of cb bytes of data from
    the pbBuff. The pcb points to the location receiving the number of bytes
    actually transferred out. Returns zero on success, or nonzero if cb is
    zero. Throws an exception on file error. In this usage, dwCookie
    contains the handle of an open file.
*-----------------------------------------------------------------------------*/
{
    // Required for StreamOut termination
    if(!cb)
        return (1);

    *pcb = 0;
    if (!::WriteFile((HANDLE)(DWORD_PTR)dwCookie, pbBuff, cb,
        (LPDWORD)pcb, NULL))
        ::MessageBox(NULL, _T("StreamOutFile Failed"), _T(""), MB_OK);
    return 0;
}

/*============================================================================*/
    BOOL    CRichEditView::
DoPreparePrinting(CPrintInfo& info)                 /*

    Call this function from your override of OnPreparePrinting to invoke
    the print dialog box to create a printer device context. This method
    invokes a print dialog box using the values in the info structure. The
    m_pPD member of this structure must contain a valid CPrintDialog object
    reference upon entry.

    After the dialog box closes with an IDOK, the a printer device context
    contains the settings specified in the dialog box and deposits this
    information so as to be available through the info parameter. This
    device context is used to print the document. Return TRUE if printing
    can begin; FALSE if the operation is canceled.
*-----------------------------------------------------------------------------*/
{
      // set up a dialog to choose the printer and printing parameters
    ::IsWindow(*info.m_pPD);
      // get the printing parameters
    if (info.m_pPD->DoModal(GetApp().GetMainWnd()) != IDOK)
        return FALSE;

      // save the printer dialog parameters so they are available
      // after the printer dialog object goes out of scope.
    PRINTDLG pd = info.m_pPD->GetParameters();
    info.SetNCopies(pd.nCopies);
    info.SetFromPage(pd.nFromPage);
    info.SetMinPage(pd.nMinPage);
    info.SetMaxPage(pd.nMaxPage);
    info.SetToPage(pd.nToPage);
    info.m_strPageDesc = _T("Page %u");
    return TRUE;
}

/*============================================================================*/
    void CRichEditView::
DoPrintView()                               /*

    Perform standard printing of a customized view of the current document,
    whose name appears in m_sDocPath, which should be set prior to entry to
    label the spooling object. If banding is supported, this should also be
    set. If the printer uses a data type to record the print job, this may
    also be set. The printing device is placed in MM_TEXT mode, so units
    are measured in twips.
*-----------------------------------------------------------------------------*/
{
    try
    {
          // maintain the printer context information throughout the
          // job via this object:
        CPrintInfo info;
          // display printer dialog and create printer device context
        if (!OnPreparePrinting(info))
            return;

          // set the printer device context via saved info
        CDC DC = info.m_pPD->GetPrinterDC();
          // put the printer DC in MM_TEXT mode and compute the page
          // m_rectDraw in twips
        DC.SetMapMode(MM_TEXT);
        int nHorizRes   = DC.GetDeviceCaps(HORZRES);
        int nVertRes    = DC.GetDeviceCaps(VERTRES);
        int nLogPixelsX = DC.GetDeviceCaps(LOGPIXELSX);
        int nLogPixelsY = DC.GetDeviceCaps(LOGPIXELSY);
        info.m_rectDraw.SetRect(0, 0, (nHorizRes * 1440) / nLogPixelsX,
            (nVertRes  * 1440) / nLogPixelsY);
          // allocate resources and perform initializations
        OnBeginPrinting(DC, info);

          // Set up the print job
        DOCINFO di;
        ZeroMemory(&di, sizeof(di));
        di.cbSize       = sizeof(DOCINFO);
        di.lpszDocName  = m_sDocPath; // the spooler label
        di.lpszOutput   = (m_sPrintPath.IsEmpty() ?
            NULL : m_sPrintPath.c_str());
        di.lpszDatatype = (m_sDataType.IsEmpty() ?
            NULL : m_sDataType.c_str());
        di.fwType       = (m_bAppBanding ?  DI_APPBANDING : 0);
          // Start the document.
        DC.StartDoc(&di);

          // Determine the printing page range
        UINT nEndPage   = info.GetToPage(),
             nStartPage = info.GetFromPage();

          // assure that the starting page is within limits
        if (nStartPage < info.GetMinPage())
            nStartPage = info.GetMinPage();
        if (nStartPage > info.GetMaxPage())
            nStartPage = info.GetMaxPage();
          // assure that the ending page is within limits
        if (nEndPage < info.GetMinPage())
            nEndPage = info.GetMinPage();
        if (nEndPage > info.GetMaxPage())
            nEndPage = info.GetMaxPage();
          // determine the page increment
        int nStep = (nEndPage >= nStartPage) ? 1 : -1;
          // adjust the end page for the for loop
        nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;
         // begin the page printing loop
        BOOL bError = FALSE;
        for (info.m_nCurPage = nStartPage; info.m_nCurPage != nEndPage;
            info.m_nCurPage += nStep)
        {
            OnPrepareDC(DC, info);

            // check for end of print
            if (!info.m_bContinuePrinting)
                break;

              // TODO: print the current page number in a progress
              // status area

              // attempt to start the current page
            if (DC.StartPage() < 0)
            {
                bError = TRUE;
                break;
            }

              // we must now call OnPrepareDC again because
              // StartPage has reset the device attributes.
#if WINVER > 0x0400
            OnPrepareDC(DC, info);
#endif
            assert(info.m_bContinuePrinting);

             // the page has been successfully started, so now
             // render the page
            OnPrint(DC, info);
            if (DC.EndPage() < 0 )
            {
                bError = TRUE;
                break;
            }
        }
          // cleanup the document printing process
        if (!bError)
            DC.EndDoc();
        else
            DC.AbortDoc();
          // and end the job
        OnEndPrinting(DC, info);
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        ::MessageBox(0, _T("Unable to display print dialog"),
            _T("Print Failed"), MB_OK);
        return;
    }
}

/*============================================================================*/
    void CRichEditView::
DoPrintRichView(const CString& sDocPath)                /*

    Print the contents of the CRichEditView control in the CView client
    area accessed by pView. Label the spooler output using the sDocPath.
*-----------------------------------------------------------------------------*/
{
      // record the document interfaces
    m_sDocPath = sDocPath;
      // let the base class administer the printing
    DoPrintView();
}

/*============================================================================*/
    void CRichEditView::
GetPageBreaks(CPrintInfo& info)                     /*

    Calculate the vector of first characters on each page in this text rich
    edit view. Set the page count and page number limits into the info
    object.
*-----------------------------------------------------------------------------*/
{
      // Use a separate format range object for pagination. This is
      // necessary because the FormatRange function alters its content
    FORMATRANGE fr = m_fr;
      // set up to scan the entire rich edit view
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;
    long nTextScanned = 0;  // amount of document scanned so far
      // find the beginning characters of each page
    m_page_first_char.clear();
      // first page begins at 0
    m_page_first_char.push_back(0);
    do
    {     // Format as much text as can fit on a page. The return value
          // is the index of the first character on the next page. Using
          // FALSE for the wParam parameter causes the text to be
          // measured.
        nTextScanned = FormatRange(fr, FALSE);
          // record the beginning character of the next page
        m_page_first_char.push_back(nTextScanned);
          // If there is more text to format, adjust the range of
          // characters to start formatting at the first character of
          // the next page.
        if (nTextScanned < m_nTextLength)
        {
            fr.chrg.cpMin = nTextScanned;
            fr.chrg.cpMax = -1;
        }
    } while (nTextScanned < m_nTextLength);
      // tell the control to release cached information.
    FormatRange(fr, FALSE);
      // on exit, the m_page_first_char vector should contain one more
      // entry than the actual number of pages
    info.SetMinPage(1);
    info.SetFromPage(1);
    UINT maxpg = m_page_first_char.size() - 1;
    info.SetMaxPage(maxpg);
    info.SetToPage(maxpg);
}

/*============================================================================*/
    void CRichEditView::
OnBeginPrinting(CDC& DC, CPrintInfo& info)                             /*

    Initiate parameters used by the printing task. This includes setting
    the FORMATRANGE m_fr structure with the printer context, the printer
    page size and margins, the range of text to print, the beginning
    locations of each page to be printed.
*-----------------------------------------------------------------------------*/
{
    HDC hPrinterDC = DC.GetHDC();

      // Rendering to the same DC we are measuring.
    ZeroMemory(&m_fr, sizeof(m_fr));
    m_fr.hdc       = hPrinterDC;  // device to render to
    m_fr.hdcTarget = hPrinterDC;  // device to format to

      // Set up the page.
    info.m_nMargin = 200; // twips
    m_fr.rcPage.left   = m_fr.rcPage.top = info.m_nMargin;
    m_fr.rcPage.right  = info.m_rectDraw.right - info.m_nMargin;
    m_fr.rcPage.bottom = info.m_rectDraw.bottom - info.m_nMargin;

      // Set up margins all around.
    m_fr.rc.left   = m_fr.rcPage.left;
    m_fr.rc.top    = m_fr.rcPage.top;
    m_fr.rc.right  = m_fr.rcPage.right;
    m_fr.rc.bottom = m_fr.rcPage.bottom;

      // Default the range of text to print as the entire document.
    m_fr.chrg.cpMin = 0;
    m_fr.chrg.cpMax = -1;

      // Find out real size of document in characters.
    m_nTextLength = GetTextLengthEx(GTL_NUMCHARS);
    GetPageBreaks(info);
    info.m_bContinuePrinting = TRUE;
      // Default the range of text to print as the entire document.
    m_fr.chrg.cpMin = 0;
    m_fr.chrg.cpMax = -1;
}

/*============================================================================*/
    void CRichEditView::
OnEndPrinting(CDC& DC, CPrintInfo& info)                               /*

    Clean up any loose ends before ending the job.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(DC);
    UNREFERENCED_PARAMETER(info);

      // tell the control to release cached information.
    FormatRange(m_fr, FALSE);
}

/*============================================================================*/
    void CRichEditView::
OnPrepareDC(CDC& DC, CPrintInfo& info /* = NULL */)                    /*

    Make preparations for printing the next page. Here, check for an
    end-of-printing condition.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(DC);
    UNREFERENCED_PARAMETER(info);
    if (m_fr.chrg.cpMin  >=  m_nTextLength)
        info.m_bContinuePrinting = FALSE;
}

/*============================================================================*/
    BOOL CRichEditView::
OnPreparePrinting(CPrintInfo& info)                                    /*

    Declare a printer dialog box, set initial info settings, and get the
    printer parameters via the printer dialog. Return TRUE if a printer was
    chosen, FALSE if the dialog was cancelled.
*-----------------------------------------------------------------------------*/
{
      // set up the dialog to choose the printer and printing parameters
    MyPrinter PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    PrintDlg.SetBoxTitle(_T("Print contents of rich edit box."));
    info.InitInfo(&PrintDlg, 1, 0xffff, 1, 0xffff, 1);
    if (!DoPreparePrinting(info))
        return FALSE;

    return TRUE;
}

/*============================================================================*/
    void CRichEditView::
OnPrint(CDC& DC, CPrintInfo& info)                                     /*

    Print the current page indicated in info.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(DC);

    UINT page_no = info.m_nCurPage;
    long first = m_page_first_char[page_no - 1];
    m_fr.chrg.cpMin = first;
    m_fr.chrg.cpMax = -1;

      // Print the a page. The return value should be the index of the first
      // character on the next page. Using TRUE for the wParam parameter
      // causes the text to be printed.
    FormatRange(m_fr, TRUE);
    DisplayBand(m_fr.rc);
}

