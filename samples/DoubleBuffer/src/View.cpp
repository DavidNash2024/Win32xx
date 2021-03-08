/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"
#include "resource.h"

/////////////////////////////
// CView function definitions
//

// Constructor.
CView::CView()
{
}

// Creates the mask bitmap for the ball.
CBitmap CView::CreateMaskBitmap()
{
    BITMAP bm = m_blue.GetBitmapData();
    CBitmap mask;
    mask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    m_ballSize.cx = bm.bmWidth;
    m_ballSize.cy = bm.bmHeight;

    CMemDC dcMem(0);
    CMemDC dcMem2(0);

    SelectObject(dcMem, m_blue);
    CBitmap oldBitmap = dcMem2.SelectObject(mask);

    COLORREF transparent = RGB(255, 255, 255);
    SetBkColor(dcMem, transparent);
    dcMem2.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dcMem, 0, 0, SRCCOPY);
    dcMem.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dcMem2, 0, 0, SRCINVERT);
    dcMem2.SelectObject(oldBitmap);

    return mask;
}

// Called when the window is created.
int CView::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER (cs);

    m_blue.LoadBitmap(IDB_BLUE);
    m_orange.LoadBitmap(IDB_ORANGE);
    m_mask = CreateMaskBitmap();

    return 0;
}

// OnInitialUpdate is called immediately after the window is created.
void CView::OnInitialUpdate()
{
    TRACE("View window created\n");

    // Start the timer
    SetTimer(ID_TIMER, 10, 0);
}

// Redraws the balls in new positions when a timer event occurs.
LRESULT CView::OnTimer(UINT msg, WPARAM wparam, LPARAM lparam)
{
    CClientDC dc(*this);
    CRect rc = GetClientRect();

    static int x = 0;     // x position
    static int y = 0;     // y position
    static int cx = 1;    // x increment or decrement
    static int cy = 1;    // y increment or decrement

    x = x + cx;
    if (x > rc.Width() - m_ballSize.cx)
    {
        x = rc.Width() - m_ballSize.cx;
        cx = -1;
    }
    else if (x < 0)
    {
        x = 0;
        cx = 1;
    }

    y = y + cy;
    if (y > rc.Height() - m_ballSize.cy)
    {
        y = rc.Height() - m_ballSize.cy;
        cy = -1;
    }
    else if (y < 0)
    {
        y = 0;
        cy = 1;
    }

    CMemDC dcMemMask(dc);
    dcMemMask.SelectObject(m_mask);

    CMemDC dcMemOrange(dc);
    dcMemOrange.SelectObject(m_orange);

    CMemDC dcMemBlue(dc);
    dcMemBlue.SelectObject(m_blue);

    CMemDC dcMem3(dc);
    dcMem3.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
    dcMem3.SolidFill(RGB(255, 255, 255), rc);

    // Copy the orange ball to the memory DC
    dcMem3.BitBlt(x, 0, m_ballSize.cx, m_ballSize.cy, dcMemOrange, 0, 0, SRCCOPY);

    // Copy the blue ball to the memory DC with mask
    dcMem3.BitBlt(0, y, 64, 64, dcMemMask, 0, 0, SRCERASE);
    dcMem3.BitBlt(0, y, 64, 64, dcMemMask, 0, 0, SRCINVERT);
    dcMem3.BitBlt(0, y, 64, 64, dcMemBlue, 0, 0, SRCINVERT);

    // Copy the memory DC to the client DC
    dc.BitBlt(0,0, rc.Width(), rc.Height(), dcMem3, 0, 0, SRCCOPY);

    return FinalWindowProc(msg, wparam, lparam);
}

// Set the WNDCLASS parameters before the window is created.
void CView::PreRegisterClass(WNDCLASS& wc)
{
    // Here we set the Window class parameters.
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the type of window we create.

    // Set the Window Class name
    wc.lpszClassName = _T("Win32++ View");

    // Set a background brush to white
    wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

    // Set the default cursor
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);

    // Set the class style (not to be confused with the window styles set in PreCreate)
    wc.style = CS_DBLCLKS;  // Generate left button double click messages
}

// All window messages for this window pass through WndProc
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_TIMER:  return OnTimer(msg, wparam, lparam);
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}



