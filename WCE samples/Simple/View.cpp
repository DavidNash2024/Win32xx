/////////////////////////////
// View.cpp


#include "view.h"
#include "resource.h"


// Called when the window's client area is drawn.
void CView::OnDraw(CDC& dc)
{
    // Define a rectangle to contain some text
    CRect rc = GetClientRect();
    rc.bottom = rc.bottom/2;

    // Draw some text in our rectangle
    LPCTSTR string = TEXT("Use Debug mode. Click here");
    dc.DrawText(string, lstrlen(string), rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

// Handle the view window's messages.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_DESTROY:
            // Terminate the application.
            ::PostQuitMessage(0);
            return 0;
        case WM_LBUTTONDOWN:
            // NOTE: You must be in debug mode to see the trace output
            TRACE("Left mouse button down message\n");
            break;
        case WM_LBUTTONUP:
            // NOTE: You must be in debug mode to see the trace output
            TRACE("Left mouse button up message\n");
            break;
    }

    // Pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

