/* (14-Nov-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                 (View.cpp) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CView class for this sample progam 
    using the Win32++ Windows interface classes, Copyright (c) 2005-2020 David 
    Nash, under permissions granted therein.

    Caveats: These materials are available under the same provisions as
    found in the Win32++ copyright.txt notice.

    Special Conventions: This class displays the client rectangle filled
    with lines of text obtained from the CDoc class. Variable horizontal tabs
    settings are not supported by this class, but use the CDoc class default.
    The size of the client is is set by the CMainFrame class, and may be
    altered by dragging the frame sides or lower-right corner.

    Vertical scrolling selects the starting line of the document and
    horizontal scrolling selects the starting character column that appear
    in the upper left hand corner of the client window. This window uses
    the MM_TEXT mapping mode as the logical unit for measurement, in which
    each logical unit is converted to one device pixel. Positive horizontal
    is to the right; positive vertical is down. Character width and line
    heights are adapted to the corresponding sizes of the current font, and
    the horizontal and vertical extents are computed as the numbers of
    characters and lines that will fit the client area.

    The display features wide characters, and is therefore able to show
    Unicode code point glyphs.

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "StdAfx.h"
#include "StdApp.h"

/*============================================================================*/
    CView::
CView()                                                                   /*

    Construct a scrollable view with colored text and background.
*-----------------------------------------------------------------------------*/
{
    SetDefaults();

      // This is an optional text message displayed at the end of the file view.
      // Set this to null if a message isn't required;    
    m_endOfView = L"---- end of document ----";
}

/*============================================================================*/
    void CView::
ClientFontChoice()                                                          /*

    Engage the font dialog to choose a font for the client window.
*-----------------------------------------------------------------------------*/
{
      // set font metric to pixels
    m_fontEx.Choose(_T("Select Font..."));
    SetAppSize(TRUE);    // document extents will change
    Invalidate();
}

/*============================================================================*/
    void CView::
InitViewColors()                                                            /*

    Initialize the custom color table entries to default values. Override
    this member to provide an application-dependent array.
*-----------------------------------------------------------------------------*/
{
    SetWndBkColor(RGB(255, 255, 255));
      // initial values for custom colors in color dialog
    m_colors[0] = RGB(0, 5, 5);
    m_colors[1] = RGB(0, 15, 55);
    m_colors[2] = RGB(0, 25, 155);
    m_colors[3] = RGB(0, 35, 255);
    m_colors[4] = RGB(10, 0, 5);
    m_colors[5] = RGB(10, 20, 55);
    m_colors[6] = RGB(10, 40, 155);
    m_colors[7] = RGB(10, 60, 255);
    m_colors[8] = RGB(100, 5, 5);
    m_colors[9] = RGB(100, 25, 55);
    m_colors[10] = RGB(100, 50, 155);
    m_colors[11] = RGB(100, 125, 255);
    m_colors[12] = RGB(200, 120, 5);
    m_colors[13] = RGB(200, 150, 55);
    m_colors[14] = RGB(200, 200, 155);
    m_colors[15] = RGB(200, 250, 255);
}

/*============================================================================*/
    void CView::
OnColorChoice()                                                             /*

    Choose the client area background color.
*-----------------------------------------------------------------------------*/
{
    CColorExDialog cd(GetWndBkColor());
      // setup the CColorChoice object
    cd.SetCustomColors(m_colors);
    cd.SetBoxTitle(_T("Select background color..."));
    if (cd.DoModal(*this) == IDOK)
    {
        SetWndBkColor(cd.GetColor());
        memcpy(m_colors, cd.GetCustomColors(), 16 * sizeof(COLORREF));
    }
}

/*============================================================================*/
    void CView::
OnInitialUpdate()                                                           /*

    Called immediately after the window is created to prepare the initial
    view.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    void    CView::
OnDraw(CDC& memDC)                                                          /*

    Paint the window's compatible bitmap whose device context is memDC
    bounded by the rectangle rc, in pixel units. The content of the window
    consists of lines of wide character text, where the top-left corner of
    the display is given as the current scroll position in the number of
    average character widths from the left and lines from the top of the
    document.
*-----------------------------------------------------------------------------*/
{
      // select the window font
    memDC.SelectObject(m_fontEx.GetFont());
      // set default text foreground and background colors
    memDC.SetTextColor(m_fontEx.GetTxColor());
    memDC.SetBkColor(GetWndBkColor());
      // display the view content
    CStringW s;
    UINT  doc_length = TheDoc().GetLength();
    if (doc_length > 0)
    {
          // get scroll bar current position: sp is the
          // upper-left-most character in the display
        CStringW fmt(L"%.4u | "),
            line_no;
          // display the current view
        for (UINT i = 0; i <= doc_length; i++)
        {
            if (m_showNumbers)
                line_no.Format(fmt, i + 1);
            if (i == doc_length)
            {
                if (m_endOfView != NULL) 
                    TextLineOut(memDC, 0, i, m_endOfView);
            }
            else
                TextLineOut(memDC, 0, i, line_no + TheDoc().GetRecord(i));
        }
    }
    else
    {
          // There is nothing to display
        s = L"No document is open.";
        CPoint sp(5, 5);
        TextLineOut(memDC, sp.x, sp.y, s);
    }
}

/*============================================================================*/
    LRESULT  CView::
OnPaint(UINT, WPARAM, LPARAM)                                               /*

    Called when part or all of the window needs to be redrawn.
*-----------------------------------------------------------------------------*/
{
    // The drawing of the client area is done in a manner that reduces
    // flicker when the window is scrolled or resized. This is done by
    // creating a compatible DC and bitmap, painting what is to appear
    // in the window into this bitmap, and, once complete, copying this
    // bitmap into the window.  This process is split into two parts: The
    // part that comprises this method is application-independent. The
    // remainder is relegated to the Paint() method. The WindProc()
    // function further responds to the WM_ERASEBKGND by not erasing the
    // background. We start by creating the device context for handling
    // the WM_PAINT message.

    CRect rc = GetClientRect(); // device units (pixels)
      // create a memory device context to print into
    CPaintDC dc(*this);
    CMemDC memDC(dc);
      // Create a bitmap big enough for our client rectangle.
    memDC.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
    memDC.FillRect(rc, GetScrollBkgnd());
      // here we would paint the client rc with app-dependent stuff
    OnDraw(memDC);
    // now we can rapidly copy the painted memory into the screen DC.
    dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), memDC, 0, 0,
        SRCCOPY);
    return 0;
}

/*============================================================================*/
    void CView::
PreCreate(CREATESTRUCT &cs)                                                 /*

    Set defaults used by the create function for the view window for
    precise control over the window created.
*-----------------------------------------------------------------------------*/
{
      // Set extended style to include a 3-D look with border and sunken edge
    cs.dwExStyle = WS_EX_CLIENTEDGE;
    CScrollView::PreCreate(cs);
}

/*============================================================================*/
    void CView::
Serialize(CArchive &ar)                                                     /*

        Serialize or deserialize the view to and from the archive ar, depending
    on the sense of IsStoring().  Leave the archive open for for further
    serialization.
*-----------------------------------------------------------------------------*/
{
      // an object for loading and storing the custom colors
    ArchiveObject ao(m_colors, 16 * sizeof(COLORREF));
    if (ar.IsStoring())
    {
          // save the current font
        ar << m_fontEx;
          // save background color
        ar << m_bkColor;
          // save the custom colors
        ar << ao;
    }
    else    // recovering
    {
          // recover the font
        ar >> m_fontEx;
        SetScrollPosition(CPoint(0, 0));
          // recover the background color
        ar >> m_bkColor;
        SetWndBkColor(m_bkColor);
          // recover the custom colors
        ar >> ao;
    }
}

/*============================================================================*/
    void CView::
SetDefaults()                                                               /*

    Set the window default parameters.
*-----------------------------------------------------------------------------*/
{
      // set font metric to pixels
    m_fontEx.SetDefault();
      // define initial background and custom colors
    InitViewColors();
    m_showNumbers = FALSE;
    SetScrollPosition(CPoint(0, 0));
}

/*============================================================================*/
    void CView::
SetAppSize(BOOL keepPos)                                                    /*

    Calculate the nominal numbers of horizontal characters and vertical
    lines that span the document in display. Set scrolling parameters.
    Retain the current position if keepPos is TRUE; otherwise, rescale the
    position in proportion to the new total scroll size.
*-----------------------------------------------------------------------------*/
{
    if (TheDoc().GetLength() == 0)
    {
        SetScrollSizes(CSize(0, 0));
        SetScrollParameters();
        return;
    }

      // select the current font into the view's device context
    CClientDC dc(*this);
    dc.SelectObject(m_fontEx.GetFont());
    CSize fs = GetFontSize();
      // get the document overall width and length
    CSize extent;
    extent.cx = fs.cx * TheDoc().GetWidth();
    extent.cy = fs.cy * TheDoc().GetLength();
      // Add 7 characters for line numbers.
      // Add 1 line for the end_of_view, if one is defined
    CSize extra((m_showNumbers ? 7 * fs.cx : 0), (m_endOfView ? fs.cy : 0));
      // to restore the scroll position
    CSize oldTSize = GetTotalScrollSize();
      // set scrolling page size to half the current view size as an arbitrary
      // but reasonable choice
    CPoint sp = GetScrollPosition();
    SetScrollSizes(extent + extra);
    SetScrollParameters();

    if (keepPos)
    {
        if (oldTSize.cx != 0 && oldTSize.cy != 0)
        {
            CSize newTSize = GetTotalScrollSize();
            sp.x = int((ULONGLONG(newTSize.cx) * ULONGLONG(sp.x)) / oldTSize.cx);
            sp.y = int((ULONGLONG(newTSize.cy) * ULONGLONG(sp.y)) / oldTSize.cy);
        }
        SetScrollPosition(sp);
    }
}

/*============================================================================*/
    void    CView::
SetScrollParameters()                                                       /*

    Compute the scroll ranges and increments for the current app and view.
*-----------------------------------------------------------------------------*/
{
    CRect rc = GetClientRect();
    CSize pageSize(rc.Width() / 2, rc.Height() / 3);
    CSize lineSize = GetFontSize();
    SetScrollSizes(GetTotalScrollSize(), pageSize, lineSize);
}

/*============================================================================*/
    void CView::
TextLineOut(CDC& dc, UINT leftcol, UINT line, LPCWSTR s) const              /*

    Output the wide character string s beginning at leftcol on the given line
    of the client area with device context dc, within the client rectangle rc
    using the given font sizes.
*-----------------------------------------------------------------------------*/
{
      // set TextOut() below to use device coordinates
    CPoint sPos = GetScrollPosition();
      // convert left column and line number to device coordinates
    CPoint pt(leftcol, line * GetFontSize().cy);
      // output the line to the view dc
    TextOutW(dc, pt.x - sPos.x, pt.y - sPos.y, s, lstrlenW(s));
}

/*============================================================================*/
    BOOL    CView::
ToggleLineNumbers()                                                     /*

    Toggle the display of line numbering in the listing of file contents.
*-----------------------------------------------------------------------------*/
{ 
    m_showNumbers = !m_showNumbers;
    SetAppSize(TRUE);
    return m_showNumbers; 
}
/*----------------------------------------------------------------------------*/

