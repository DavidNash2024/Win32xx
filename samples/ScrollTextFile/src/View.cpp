/* (14-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                 (View.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2017, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CScrollWnd demo application
    view class for a typical Windows application using the Win32++ Windows
    interface classes, Copyright (c) 2005-2017 David Nash, under
    permissions granted therein. This class displays the contents
    of a document in the form of a scrollable set of lines of text.

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

================================================================================

    Special Conventions: This class displays the client rectangle filled with
    lines of text obtained from the CDoc class. Horizontal tabs are not
    supported by this class; such padding is expected to be supplied by the
    CDoc class. The size of the client are is set by the CMainFrame class,
    and is resizable by dragging the frame sides or lower-right corner.

    Vertical scrolling selects the starting line of the document and
    horizontal scrolling selects the starting character column that appear
    in the upper left hand corner of the client window. This window uses the 
    MM_TEXT mapping mode as the logical unit for measurement, in which each 
    logical unit is converted to one device pixel. Positive horizontal is to 
    the right; positive vertical is down. Character width and line heights 
    are adapted to the corresponding sizes of the current font, and the 
    horizontal and vertical extents are computed as the numbers of characters 
    and lines that will fit the client area. The translation between logical
    (character, line) coordinates and (hPixel, yPixel) device coordinates is 
    done via the ScrlToDev() and DevToScrl() members.
    
    For particulars, see the methods

        DevToScrl
        SetNewAppSize()
        ScrlToDev()
        TextLineOut()

================================================================================

    Programming Notes:

    Typical usage of the CView class is found in the CMainFrame 
    functions

        CMainFrame
        OnColorChoice
        OnCreate
        OnFileClose
        OnFileOpen
        OnFileOpenMRU
        SaveRegistry

    Interfaces between CView and CDoc appear in

        SetNewAppSize
        Paint

    The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "stdapp.h"

  // Define a special characters-and-lines logical mapping mode for this
  // view.
#define MM_CHARS_LINES (MM_MIN -1)

/*============================================================================*/
    CView::
CView()                                                         /*

    Construct a scrollable view with colored text and background.
*-----------------------------------------------------------------------------*/
{
    SetMappingMode(MM_CHARS_LINES);
    m_VuFont = CFontEx(rgbDefaultTxColor, CF_SCREENFONTS);
      // define initial custom colors
    InitCustomColors();
}

/*============================================================================*/
    CView::
~CView()                                                        /*

    Destructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    CPoint  CView::
DevToScrl(CPoint devpt) const                           /*

    Transform the scrolling coordinate device point devpt (here, pixels) 
    to the corresponding scroll position(here, chars and lines). 
*-----------------------------------------------------------------------------*/
{
    assert(GetMappingMode() == MM_CHARS_LINES);
    return CPoint(devpt.x / m_VuFont.GetWidth(), 
              devpt.y / m_VuFont.GetHeight());
}

/*============================================================================*/
    void CView::
ClientFontChoice()                          /*

    Engage the font dialog to choose a font for the client window.
*-----------------------------------------------------------------------------*/
{
    CClientDC dc(*this);
      // set font metric to pixels
    dc.SetMapMode(MM_TEXT);
    m_VuFont.Choose(dc, _T("Select Font..."));
    SetNewAppSize();    // document extents will change
    SetNewViewSize();   // logical view width will change
    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    void CView::
InitCustomColors()                                                      /*

    Initialize the custom color table entries to default values. Override
    this member to provide an application-dependent array.
*-----------------------------------------------------------------------------*/
{
    // initial values for custom colors in color dialog
    m_rgbCustomColors[0]  = RGB(0,    5,   5);
    m_rgbCustomColors[1]  = RGB(0,    15,  55);
    m_rgbCustomColors[2]  = RGB(0,    25, 155);
    m_rgbCustomColors[3]  = RGB(0,    35, 255);
    m_rgbCustomColors[4]  = RGB(10,    0,   5);
    m_rgbCustomColors[5]  = RGB(10,   20,  55);
    m_rgbCustomColors[6]  = RGB(10,   40, 155);
    m_rgbCustomColors[7]  = RGB(10,   60, 255);
    m_rgbCustomColors[8]  = RGB(100,   5,   5);
    m_rgbCustomColors[9]  = RGB(100,  25,  55);
    m_rgbCustomColors[10] = RGB(100,  50, 155);
    m_rgbCustomColors[11] = RGB(100, 125, 255);
    m_rgbCustomColors[12] = RGB(200, 120,   5);
    m_rgbCustomColors[13] = RGB(200, 150,  55);
    m_rgbCustomColors[14] = RGB(200, 200, 155);
    m_rgbCustomColors[15] = RGB(200, 250, 255);
}

/*============================================================================*/
    void CView::
OnColorChoice()                             /*

    Choose the client area background color.
*-----------------------------------------------------------------------------*/
{
    CColorExDialog cd(GetWndBkColor());
      // setup the CColorChoice object
    cd.SetCustomColors(m_rgbCustomColors);
    cd.SetBoxTitle(_T("Select background color..."));
    if(cd.DoModal(*this) == IDOK)
    {
        SetWndBkColor(cd.GetColor());
        memcpy(m_rgbCustomColors, cd.GetCustomColors(), 
            16 * sizeof(COLORREF));
    }
}

/*============================================================================*/
    void CView::
OnInitialUpdate()                                                       /*

    Called immediately after the window is created to prepare the initial 
    view.
*-----------------------------------------------------------------------------*/
{
      // let the base class initialize
    CScrollWnd::OnInitialUpdate();
}

/*============================================================================*/
        void    CView::
Paint(CDC& dcMem)                                               /*

    Paint the window's compatible bitmap whose device context is dcMem
    bounded by the rectangle rc, in pixel units. The content of the window
    consists of lines of text, where the top-left corner of the display is
    given as the current scroll position in the number of average character
    widths from the left and lines from the top of the document.
*-----------------------------------------------------------------------------*/
{
      // select the window font
    dcMem.SelectObject(m_VuFont.GetFont());
      // set default text foreground and background colors
    dcMem.SetTextColor(m_VuFont.GetTxColor());
    dcMem.SetBkColor(GetWndBkColor());
      // display the view content
    CString s;
    if (TheDoc().GetLength() > 0)
    {
        int  doc_length = TheDoc().GetLength(),
             vu_length  = GetClientSize().cy;
          // get scroll bar current position: sp is the 
          // upper-left-most character in the display
        CPoint sp = GetScrollPosition();
          // display the current view
        for (int i = 0; i < vu_length; i++)
        {
            int j = i + sp.y;
            if (j > doc_length)
                continue;

            else if (j == doc_length)
                  // for the demo only:
                s = _T("---- end of document ----");
            else
                s =  TheDoc().GetRecord(j, sp.x); 

            TextLineOut(dcMem, 0, i, s.c_str());
        }
    }
    else
    {
          // There is no image, so display a hint to get one
        ShowHScrollBar(FALSE);
        ShowVScrollBar(FALSE);
        s = _T("No document is open.");
        CPoint sp(5, 5);
        TextLineOut(dcMem, sp.x, sp.y, s.c_str());
    }
}

/*============================================================================*/
    void CView::
PreCreate(CREATESTRUCT &cs)                                             /*

    Set defaults used by the create function for the view window for
    precise control over the window created.
*-----------------------------------------------------------------------------*/
{
    CClientDC dc(*this);
      // set font metric to pixels
    dc.SetMapMode(MM_TEXT);
    m_VuFont.SetDefault(dc);

      // Set the extended style to include a 3-D look with border and
      // sunken edge
    cs.dwExStyle = WS_EX_CLIENTEDGE;
    CScrollWnd::PreCreate(cs);
}

/*============================================================================*/
    void CView::
PreRegisterClass(WNDCLASS &wc)                                          /*

    Set Window class parameters for precise control over the characteristics
    of the window class created.
*-----------------------------------------------------------------------------*/
{
      // Set the class style (not to be confused with the window styles
      // set in PreCreate) to generate left button double-click messages
    wc.style = CS_DBLCLKS;  
    CScrollWnd::PreRegisterClass(wc);
}

/*============================================================================*/
    BOOL CView::
PreTranslateMessage(MSG &msg)                                           /*

    Used by CWinApp to translate window messages before they are dispatched
    to theTranslateMessage and DispatchMessage Windows functions in the
    message loop. MSG contains the message to process. Return a nonzero
    if the message was translated and should not be dispatched; return
    0 if the message was not translated and should be dispatched.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(msg);

//  HWND   hwnd = Msg->hwnd;
//  UINT   message  = Msg->message;
//  WPARAM wParam   = Msg->wParam;
//  LPARAM lParam   = Msg->lParam;
//  DWORD  time = Msg->time;
//  CPoint  pt  = Msg->pt;

      // return base class value if the message was NOT handled here
    return CScrollWnd::PreTranslateMessage(msg);
}

/*============================================================================*/
    CPoint  CView::
ScrlToDev(CPoint viewpt) const                      /*

    Transform the scrolling coordinate point viewpt (here, char and line) 
    to the corresponding device point (here, client view pixels). 
*-----------------------------------------------------------------------------*/
{
    assert(GetMappingMode() == MM_CHARS_LINES);
    return CPoint(viewpt.x * m_VuFont.GetWidth(), 
              viewpt.y * m_VuFont.GetHeight());
}

/*============================================================================*/
        void CView::
Serialize(CArchive &ar)                                                 /*

        Serialize or deserialize the view to and from the archive ar, depending
    on the sense of IsStoring().  Leave the archive open for for further
    serialization.
*-----------------------------------------------------------------------------*/
{
      // recover base class data first
        CScrollWnd::Serialize(ar);
      // perform loading or storing
        LOGFONT lf;
    ArchiveObject f(&lf, sizeof(LOGFONT));
    ArchiveObject ao(m_rgbCustomColors, 16 * sizeof(COLORREF));
        if (ar.IsStoring())
        {
                  // save m_hfFont
        ar << m_VuFont;
              // save the custom colors
        ar << ao;   
    }
        else    // recovering
        {
                  // recover m_hfFont
        ar >> m_VuFont;
        SetScrollPosition(CPoint(0, 0));
              // recover the custom colors
        ar >> ao;   
        }
}

/*============================================================================*/
        void    CView::
SetScrollAppInfo()                          /*

    Set the scroll ranges and increments for this document.
*-----------------------------------------------------------------------------*/
{
      // set scroll limits to columns and lines in the document
    int hPage = GetViewSize().cx / 2;
    SetScrollRanges(0, GetAppSize().cx - hPage, 0, GetAppSize().cy);
      // set scroll increments based on client window size
    SetScrollIncrements(1, hPage, 1, GetViewSize().cy - 1);
}

/*============================================================================*/
    void CView::
SetNewAppSize()                                             /*

    Calculate the nominal numbers of horizontal characters and vertical 
    lines that span the document in display. Set scrolling parameters.
*-----------------------------------------------------------------------------*/
{
    if (TheDoc().GetLength() == 0)
    {
        SetAppSize(CSize(0, 0));
        return;
    }

      // select the current font into the view's device context
    CClientDC dc(*this);
    dc.SetMapMode(MM_TEXT);
    dc.SelectObject(m_VuFont.GetFont());
      // get the document overall width and length
    CSize extent = TheDoc().GetMaxExtent(dc);
    CPoint pt(extent.cx, extent.cy);
    pt = DevToScrl(pt); // now in logical coordinates
    SetAppSize(CPoint(pt.x, TheDoc().GetLength()));
    SetScrollParameters();
}

/*============================================================================*/
        void    CView::
TextLineOut(CDC& dc, UINT leftcol, UINT line, CString s)        /*

    Output the string s beginning at leftcol on the given line of the client
    area with device context dc, within the client rectangle rc using the
    given font sizes.
*-----------------------------------------------------------------------------*/
{
      // set TextOut() below to use device coordinates
    int oldmode = dc.SetMapMode(MM_TEXT);
    UINT textalign = dc.GetTextAlign(); // current text alignment flags
    SetTextAlign(dc, TA_NOUPDATECP);    // do not update current position
      // convert left column and line number to device coordinates
    CPoint pt = ScrlToDev(CPoint(leftcol, line));
      // output the line to the view dc
    TextOut(dc, pt.x, pt.y, s.c_str(), s.GetLength());
      // restore previous alignment flag status and mapping mode
    SetTextAlign(dc, textalign);
    dc.SetMapMode(oldmode);
}

/*============================================================================*/
    LRESULT CView::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                        /*

    All messages for this window pass through this WndProc.  In particular,
    handling scrollbar messages are dispatched here.
*-----------------------------------------------------------------------------*/
{
    //switch (uMsg)
    //{
    //    case WM_LBUTTONDOWN:  // user clicks in the client area
    //    {
    //    // let the main frame OnCommand() handle this as a
    //    // notification from the main window
    //      return TheApp().TheFrame().SendMessage(WM_COMMAND,
    //      (WPARAM)IDW_MAIN, (LPARAM)0);
    //    }
    //}

      // return base class value if NOT processed here
    return CScrollWnd::WndProc(uMsg, wParam, lParam);
}

