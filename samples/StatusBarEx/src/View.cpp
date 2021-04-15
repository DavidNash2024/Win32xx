/* [Tab/Indent: 8/8][Line/Box: 80/74]                               (View.cpp) *
********************************************************************************

    Contents Description: Implementation of the CView class for a sample
    program demonstrating the extended use of the status bar using the
    Win32++ Windows framework.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "View.h"

/*============================================================================*/
    void CView::
OnDraw(CDC& dc)                                                             /*

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
OnInitialUpdate()                                                           /*

    This method is called immediately after the window is created.
*-----------------------------------------------------------------------------*/
{
    TRACE("View window created\n");
}

/*============================================================================*/
    void CView::
PreCreate(CREATESTRUCT& cs)                                                 /*

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
PreRegisterClass(WNDCLASS& wc)                                              /*

    Set the Window class parameters. Preforming this is optional, but
    doing so allows more precise control over the type of window created.
*-----------------------------------------------------------------------------*/
{
      // Set the Window Class name
    wc.lpszClassName = _T("Win32++ View");

      // Set a background brush to white
    wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

      // Set the default cursor
    wc.hCursor = ::LoadCursor(0, IDC_ARROW);

      // Set the class style (not to be confused with the window styles
      // set in PreCreate)
    wc.style = CS_DBLCLKS;  // Generate left button double click messages
}

/*============================================================================*/
    LRESULT CView::
WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                              /*

    All messages for this window pass through this process.
*-----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------*/
