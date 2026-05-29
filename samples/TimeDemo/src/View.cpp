/////////////////////////////////////////
// View.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "View.h"
#include "Doc.h"

// The latest file compilation date.
const CString CView::m_compiledOn = __DATE__;

//////////////////////////////
// CView function definitions.
//

// Construct a scrollable view with colored text and background.
CView::CView()
{
    SetDefaults();
}

// Retrieves the size of the current font's characters in pixels.
CSize CView::GetFontSize() const
{
    // Select the current font into a temporary device context.
    CClientDC dc(HWND_DESKTOP);
    dc.SelectObject(m_font);

    // Measure the font width and height.
    TEXTMETRIC tm;
    dc.GetTextMetrics(tm);
    CSize fontSize;
    fontSize.cx = tm.tmAveCharWidth;
    fontSize.cy = tm.tmHeight + tm.tmExternalLeading;
    return fontSize;
}

// Called by OnPaint.This function draws to the window's compatible
// bitmap whose device context is dcMem.The content of the window
// consists of lines of wide character text, where the top - left corner of
// the display is given as the current scroll position in the number of
// average character widths from the left and lines from the top of the
// document.
void CView::OnDraw(CDC& dcMem)
{
    // Select the window font.
    dcMem.SelectObject(m_font);

    // Set default text foreground and background colors.
    dcMem.SetTextColor(m_textColor);
    dcMem.SetBkColor(m_bkgndColor);

    // Display the view content.
    UINT  doc_length = TheDoc().GetDocLength();
    if (doc_length > 0)
    {
        // Display the current view.
        for (UINT i = 0; i <= doc_length; i++)
        {
            TextLineOut(dcMem, 0, i, TheDoc().GetDocRecord(i));
        }
    }
    else
    {
        // There is nothing to display.
        CString s = L"No document is open.";
        CPoint sp(5, 5);
        TextLineOut(dcMem, sp.x, sp.y, s);
    }
}

// Called when part or all of the window needs to be redrawn. Here we override
// CScrollView::OnPaint and provide our own bitmap.This avoids the scrolling
// issues which would be caused if we were to draw on specific locations while
// the window is being scrolled.This function sets the background color.It
// then calls OnDraw where the text is drawn on to the bitmap we created.
// The completed bitmap is then copied to the window's painting device context.
LRESULT  CView::OnPaint(UINT, WPARAM, LPARAM)
{
    CRect rc = GetClientRect(); // device units (pixels)

    // Create a the device contexts.
    CPaintDC dc(*this);
    CMemDC dcMem(dc);

    // Create a bitmap big enough for our client rectangle.
    dcMem.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());

    // Set the background color.
    CBrush brBkGnd(m_bkgndColor);
    dcMem.FillRect(rc, brBkGnd);

    // Draw to the memory device context.
    OnDraw(dcMem);

    // Copy from the memory DC to the screen DC.
    dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), dcMem, 0, 0, SRCCOPY);
    return 0;
}

// Set defaults used by the create function for the view window for
// precise control over the window created.
void CView::PreCreate(CREATESTRUCT &cs)
{
    // Set extended style to include a 3-D look with border and sunken edge.
    cs.dwExStyle = WS_EX_CLIENTEDGE;
    CScrollView::PreCreate(cs);
}

// Calculate the nominal numbers of horizontal characters and vertical
// lines that span the document in display.Set scrolling parameters.
// Retain the current position if keepPos is TRUE; otherwise, rescale the
// position in proportion to the new total scroll size.
void CView::SetAppSize(BOOL keepPos)
{
    if (TheDoc().GetDocLength() == 0)
    {
        SetScrollSizes(CSize(0, 0));
        return;
    }

    // Save original scroll size and scroll position.
    CSize oldTSize = GetTotalScrollSize();
    CPoint sp = GetScrollPosition();

    // Get the document overall width and length.
    CSize extent;
    CSize fs = GetFontSize();
    extent.cx = fs.cx * TheDoc().GetDocWidth();
    extent.cy = fs.cy * TheDoc().GetDocLength();

    // Set scrolling page size to a reasonable choice.
    CRect rc = GetClientRect();
    CSize pageSize(rc.Width() / 2, rc.Height() / 3);
    CSize lineSize = GetFontSize();
    SetScrollSizes(extent, pageSize, lineSize);

    if (keepPos)
    {
        // Restore previous scroll position.
        if (oldTSize.cx != 0 && oldTSize.cy != 0)
        {
            CSize newTSize = GetTotalScrollSize();
            sp.x = int((ULONGLONG(newTSize.cx) * ULONGLONG(sp.x)) / oldTSize.cx);
            sp.y = int((ULONGLONG(newTSize.cy) * ULONGLONG(sp.y)) / oldTSize.cy);
        }

        SetScrollPosition(sp);
    }
}

// Set the member variables to their default values.
void CView::SetDefaults()
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

// Output the character string s beginning at leftcol on the given line of
// the client area with device context dc, within the client rectangle rc
// using the given font sizes.
void CView::TextLineOut(CDC& dc, UINT leftcol, UINT line, LPCWSTR s) const
{
    // Set TextOut() below to use device coordinates.
    CPoint sPos = GetScrollPosition();

    // Convert left column and line number to device coordinates.
    CPoint pt(leftcol * GetFontSize().cx, line * GetFontSize().cy);

    // Output the line to the view dc.
    TextOut(dc, pt.x - sPos.x, pt.y - sPos.y, s, lstrlen(s));
}

// Handle the window's messages.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)

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
        str2 << L"Error: " << e.what();
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

