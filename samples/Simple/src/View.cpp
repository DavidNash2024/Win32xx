/////////////////////////////
// View.cpp
//

#include "View.h"
#include "resource.h"

//////////////////////////////
// CView function definitions.
//

// OnCreate is called automatically during window creation when a
// WM_CREATE message received.
// Tasks such as setting the icon, creating child windows, or anything
// associated with creating windows are normally performed here.
int CView::OnCreate(CREATESTRUCT&)
{
    // Set the window's icon
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Set the window title
    SetWindowText(LoadString(IDW_MAIN));

    TRACE("OnCreate\n");
    return 0;
}

// Called when the window is destroyed.
void CView::OnDestroy()
{
    // End the application when the window is destroyed.
    ::PostQuitMessage(0);
}

// OnPaint is called automatically whenever a part of the
// window needs to be repainted.
void CView::OnDraw(CDC& dc)
{
    // Centre some text in our view window.
    CRect rc = GetClientRect();
    CString cs = LoadString(IDW_MAIN);
    dc.DrawText(cs, cs.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

// OnInitialUpdate is called after the window is created.
// Tasks which are to be done after the window is created go here.
void CView::OnInitialUpdate()
{
    TRACE("OnInitialUpdate\n");
}

// Called when the window is resized.
void CView::OnSize()
{
    // Force the window to be repainted during resizing.
    Invalidate();
}

// This function will be called automatically by Create. It provides an
// opportunity to set various window parameters prior to window creation.
// You are not required to set these parameters. Any parameters which
// aren't specified are set to reasonable defaults.
void CView::PreCreate(CREATESTRUCT& cs)
{
    // Set some optional parameters for the window.
    cs.dwExStyle = WS_EX_CLIENTEDGE;        // Extended style
    cs.lpszClass = _T("View Window");       // Window Class
    cs.x = 50;                              // top x
    cs.y = 50;                              // top y
    cs.cx = 400;                            // width
    cs.cy = 300;                            // height
}

// This function is our message procedure. We process the messages for
// the view window here.  Unprocessed messages are passed on for
// default processing.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_SIZE:
        OnSize();
        break;  // Also do default processing for this message.
    }

    // Pass unhandled messages on for default processing.
    return WndProcDefault(msg, wparam, lparam);
}
