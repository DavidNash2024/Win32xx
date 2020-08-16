/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "view.h"

//////////////////////////////
// CView function definitions.
//
CView::CView()
{
}

void CView::OnDraw(CDC& dc)
// OnDraw is called when part or all of the window needs to be redrawn
{
    CRect rc = GetClientRect();

    // Centre some text in our view window
    dc.DrawText(_T("View Window"), -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CView::OnInitialUpdate()
// OnInitialUpdate is called immediately after the window is created
{
    TRACE("View window created\n");
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // Here we set the defaults used by the create function for the view window
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the window we create.

    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

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

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
// All window messages for this window pass through WndProc
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



