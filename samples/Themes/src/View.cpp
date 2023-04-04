/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"

//////////////////////////////
// CView function definitions.
//
CView::CView()
{
}

void CView::OnDraw(CDC& dc)
{
    // Use the message font for Windows 7 and higher.
    if (GetWinVersion() >= 2601)
    {
        NONCLIENTMETRICS info = GetNonClientMetrics();
        LOGFONT lf = info.lfMessageFont;
        int dpi = GetWindowDPI(*this);
        lf.lfHeight = -MulDiv(10, dpi, POINTS_PER_INCH);
        dc.CreateFontIndirect(lf);
    }

    // Centre some text in our view window
    CRect rc = GetClientRect();
    dc.DrawText(_T("Choose a theme from the menu"), -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CView::OnInitialUpdate()
{
    // OnInitialUpdate is called immediately after the window is created
    TRACE("View window created\n");
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // Here we set the defaults used by the create function for the view window
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the window we create.

    // Set the Window Class name
    cs.lpszClass = _T("View");

    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:
            Invalidate();
            break;  // Also do default processing
        }

        // pass unhandled messages on for default processing
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

