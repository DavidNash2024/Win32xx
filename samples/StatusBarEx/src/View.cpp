/* [Tab/Indent: 8/8][Line/Box: 80/74]                               (View.cpp) *
********************************************************************************

    Contents Description: Implementation of the CView class for a sample
    program demonstrating the extended use of the status bar using the
    Win32++ Windows framework, Copyright (c) 2005-2017 David Nash.

    This particular program was adapted from the StatusBar sample progam
    in the Win32++ distribution by Robert C. Tausworthe to illustrate the
    definition of the status bar makeup by way of a data structure, in a
    similar fashion as that appearing in the Microsoft Foundation Classes
    (MFC), and to override the Win32++ normal statusbar definition of four
    parts to any number suitable for an application.

    The code changes to the StatusBar sample program only occur in the
    CMainFrame and MyStatusBar classes. However, the entire program has
    been reformatted for better readability, per the Programming Notes,
    below.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.

    Programming Notes:
                The programming conventions used here roughly follow those
        established by the 1997-1999 Jet Propulsion Laboratory Deep
        Space Network Planning and Preparation Subsystem project for
        C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "view.h"

/*============================================================================*/
    CView::
CView()                                 /*

*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    void CView::
OnDraw(CDC& dc)     /*

    This called when part or all of the window needs to be redrawn. For
    this sample program, merely center some token text in the view window.

*-----------------------------------------------------------------------------*/
{
    CRect rc = GetClientRect();
    dc.DrawText(_T("View Window"), -1, rc, DT_CENTER | DT_VCENTER |
        DT_SINGLELINE);
}

/*============================================================================*/
    void CView::
OnInitialUpdate()                           /*

    This method is called immediately after the window is created.
*-----------------------------------------------------------------------------*/
{
    TRACE("View window created\n");
}

/*============================================================================*/
    void CView::
PreCreate(CREATESTRUCT& cs)                     /*

    Set the defaults used by the create function for the view window.
    Preforming this is optional, but doing so allows more precise control
    over the window created.
*-----------------------------------------------------------------------------*/
{
    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

/*============================================================================*/
    void CView::
PreRegisterClass(WNDCLASS& wc)                      /*

    Sset the Window class parameters. Preforming this is optional, but
    doing so allows more precise control over the type of window created.
*-----------------------------------------------------------------------------*/
{
      // Set the Window Class name
    wc.lpszClassName = _T("Win32++ View");

      // Set a background brush to white
    wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

      // Set the default cursor
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);

      // Set the class style (not to be confused with the window styles
      // set in PreCreate)
    wc.style = CS_DBLCLKS;  // Generate left button double click messages
}

/*============================================================================*/
    LRESULT CView::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)        /*

    All messages for this window pass through this process.
*-----------------------------------------------------------------------------*/
{
    switch (uMsg)
    {
        case WM_SIZE:
        Invalidate();
        break;  // Also do default processing
    }

      // pass unhandled messages on for default processing
    return WndProcDefault(uMsg, wParam, lParam);
}



