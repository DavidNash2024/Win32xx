/* (20-Oct-2024) [Tab/Indent: 4/4][Line/Box: 80/74]                 (View.cpp) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2020                |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CView class derived from the
    CScrollView base class for this application using the Win32++ Windows
    framework.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "View.h"
#include "Doc.h"
#include "App.h"

/*******************************************************************************

    Class static variable initialization                                    */

    // latest file compilation date
const CString CView::m_compiledOn = __DATE__;

/*============================================================================*/
    CView::
CView()                                                                     /*

    Construct a scrollable view with colored text and background.
*-----------------------------------------------------------------------------*/
{
    SetDefaults();
}

/*============================================================================*/
    CSize CView::
GetFontSize() const                                                         /*

    Retrieves the size of the current font's characters in pixels.
*-----------------------------------------------------------------------------*/
{
      // select the current font into a temporary device context
    CClientDC dc(HWND_DESKTOP);
    dc.SelectObject(m_font);
      // measure the font width and height
    TEXTMETRIC tm;
    dc.GetTextMetrics(tm);
    CSize fontSize;
    fontSize.cx = tm.tmAveCharWidth;
    fontSize.cy = tm.tmHeight + tm.tmExternalLeading;
    return fontSize;
}

/*============================================================================*/
    void    CView::
OnDraw(CDC& dcMem)                                                          /*

    Called by OnPaint. This function draws to the window's compatible
    bitmap whose device context is dcMem. The content of the window
    consists of lines of wide character text, where the top-left corner of
    the display is given as the current scroll position in the number of
    average character widths from the left and lines from the top of the
    document.
*-----------------------------------------------------------------------------*/
{
      // select the window font
    dcMem.SelectObject(m_font);
      // set default text foreground and background colors
    dcMem.SetTextColor(m_textColor);
    dcMem.SetBkColor(m_bkgndColor);
    // display the view content
    UINT  doc_length = TheDoc().GetDocLength();
    if (doc_length > 0)
    {
        // display the current view
        for (UINT i = 0; i <= doc_length; i++)
        {
            TextLineOut(dcMem, 0, i, TheDoc().GetDocRecord(i));
        }
    }
    else
    {
        // There is nothing to display
        CString s = L"No document is open.";
        CPoint sp(5, 5);
        TextLineOut(dcMem, sp.x, sp.y, s);
    }
}

/*============================================================================*/
    LRESULT  CView::
OnPaint(UINT, WPARAM, LPARAM)                                               /*

    Called when part or all of the window needs to be redrawn. Here we override
    CScrollView::OnPaint and provide our own bitmap. This avoids the scrolling
    issues which would be caused if we were to draw on specific locations while
    the window is being scrolled. This function sets the background color. It
    then calls OnDraw where the text is drawn on to the bitmap we created.
    The completed bitmap is then copied to the window's painting device context.
*-----------------------------------------------------------------------------*/
{
    CRect rc = GetClientRect(); // device units (pixels)
      // create a memory device context to print into
    CPaintDC dc(*this);
    CMemDC dcMem(dc);
      // Create a bitmap big enough for our client rectangle.
    dcMem.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
      // set the background color
    CBrush brBkGnd(m_bkgndColor);
    dcMem.FillRect(rc, brBkGnd);
      // here we would paint the client rc with app-dependent stuff
    OnDraw(dcMem);
      // now we can rapidly copy the painted memory into the screen DC.
    dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), dcMem, 0, 0,
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
SetAppSize(BOOL keepPos)                                                    /*

    Calculate the nominal numbers of horizontal characters and vertical
    lines that span the document in display. Set scrolling parameters.
    Retain the current position if keepPos is TRUE; otherwise, rescale the
    position in proportion to the new total scroll size.
*-----------------------------------------------------------------------------*/
{
    if (TheDoc().GetDocLength() == 0)
    {
        SetScrollSizes(CSize(0, 0));
        return;
    }

    // Save original scroll size and scroll position
    CSize oldTSize = GetTotalScrollSize();
    CPoint sp = GetScrollPosition();

    // get the document overall width and length
    CSize extent;
    CSize fs = GetFontSize();
    extent.cx = fs.cx * TheDoc().GetDocWidth();
    extent.cy = fs.cy * TheDoc().GetDocLength();

    // set scrolling page size to a reasonable choice
    CRect rc = GetClientRect();
    CSize pageSize(rc.Width() / 2, rc.Height() / 3);
    CSize lineSize = GetFontSize();
    SetScrollSizes(extent, pageSize, lineSize);

    if (keepPos)
    {
        // restore previous scroll position
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
    void CView::
SetDefaults()                                                               /*

    Sets the member variables to their default values.
*-----------------------------------------------------------------------------*/
{
    m_textColor = RGB(0, 0, 0);
    m_bkgndColor = RGB(255, 255, 128);
    for (int i = 0; i < 16; i++)
        m_colors[i] = RGB(0, 0, 0);

      // Create the default font.
    constexpr int fontSize = 10;
    constexpr int tenthsOfPoint = 10;
    m_font.CreatePointFont(fontSize * tenthsOfPoint, L"Courier New");
}

/*============================================================================*/
    void    CView::
TextLineOut(CDC& dc, UINT leftcol, UINT line, LPCWSTR s) const              /*

    Output the character string s beginning at leftcol on the given line of
    the client area with device context dc, within the client rectangle rc
    using the given font sizes.
*-----------------------------------------------------------------------------*/
{
      // set TextOut() below to use device coordinates
    CPoint sPos = GetScrollPosition();
      // convert left column and line number to device coordinates
    CPoint pt(leftcol * GetFontSize().cx, line * GetFontSize().cy);
      // output the line to the view dc
    TextOut(dc, pt.x - sPos.x, pt.y - sPos.y, s, lstrlen(s));
}

/*============================================================================*/
LRESULT CView::
    WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                             /*

        Handle the window's messages.
    *-----------------------------------------------------------------------------*/

{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}
/*----------------------------------------------------------------------------*/

