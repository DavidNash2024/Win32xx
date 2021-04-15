/* [Tab/Indent: 8/8][Line/Box: 80/74]                          (Hyperlink.cpp) *
********************************************************************************

    Contents Description: Implementation of the CHyperlink class for a sample
    program demonstrating the custom status bar within the  Win32++ Windows
    framework.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "Hyperlink.h"

/*============================================================================*/
    CHyperlink::
CHyperlink()                                                                /*

    Create the custom hyperlink for this sample program.
*-----------------------------------------------------------------------------*/
    :   m_isUrlVisited(FALSE), m_isClicked(FALSE),
        m_crVisited(hyTextColor[0]), m_crNotVisited(hyTextColor[1])
{
      // Create the cursor
    m_cursor = ::LoadCursor(0, IDC_HAND);

      // IDC_HAND is not available on Win95, so load a reasonable
      // alternative
    if( !m_cursor )
        m_cursor = ::LoadCursor(0, IDC_ARROW);
}

/*============================================================================*/
    void CHyperlink::
OnAttach()                                                                  /*

    Set the content of the hyperlink status bar part and modify the font
    to underscore the part string.
*-----------------------------------------------------------------------------*/
{
    SetWindowText(_T("Win32++"));
    NONCLIENTMETRICS ncm = GetNonClientMetrics();
    LOGFONT lf = ncm.lfMessageFont;
    lf.lfUnderline = TRUE;
    m_urlFont.CreateFontIndirect(lf);
}

/*============================================================================*/
    LRESULT CHyperlink::
OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)                      /*

    Handle the left mouse button has been pressed down.
*-----------------------------------------------------------------------------*/
{
    SetCapture();
    m_isClicked = TRUE;
    return FinalWindowProc(msg, wparam, lparam);
}

/*============================================================================*/
    LRESULT CHyperlink::
OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)                        /*

    Handle the release of the mouse button after pressing down.
*-----------------------------------------------------------------------------*/
{
    ReleaseCapture();
    if(m_isClicked)
    {
        m_isClicked = FALSE;
        CPoint pt;
        pt.x = (short)LOWORD(lparam);
        pt.y = (short)HIWORD(lparam);
        ClientToScreen(pt);
        CRect rc = GetWindowRect();
        if (rc.PtInRect(pt))
            OpenUrl();
    }
    return FinalWindowProc(msg, wparam, lparam);
}

/*============================================================================*/
    void CHyperlink::
OpenUrl()                                                                   /*

    Respond the the selection of the hyperlink text by displaying the
    Win32++ SourceForge page in the browser.
*-----------------------------------------------------------------------------*/
{
    LPCTSTR url = _T("http://sourceforge.net/projects/win32-framework/");

    HINSTANCE result = ::ShellExecute(0, _T("open"), url, NULL, NULL,
        SW_SHOWNORMAL);

    if (reinterpret_cast<INT_PTR>(result) > 32)
    {
        m_isUrlVisited = TRUE;
          // redraw the StatusBar to update the color
        GetParent().RedrawWindow();
    }
}

/*============================================================================*/
    LRESULT CHyperlink::
OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam)                   /*

    Reflect WM_CTLCOLORSTATIC messages back here for processing to set the
    color of the hyperlink statusbar part.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);

    if (msg ==  WM_CTLCOLORSTATIC)
    {
        CDC dc(reinterpret_cast<HDC>(wparam));
        dc.SolidFill(hyBkgrColor, GetClientRect());
        dc.SetTextColor(m_isUrlVisited? m_crVisited : m_crNotVisited);
        dc.SetBkMode(TRANSPARENT);
        dc.SelectObject(m_urlFont);
          // Return a NULL brush for a transparent background
        return (LRESULT)GetStockObject(NULL_BRUSH);
    }
    return 0L;
}

/*============================================================================*/
    LRESULT CHyperlink::
OnSetCursor(UINT, WPARAM, LPARAM)                                           /*

*-----------------------------------------------------------------------------*/
{
        // Must use ::SetCursor here. CStatic::SetCursor does not do the
        // same thing.
    ::SetCursor(m_cursor);
    return TRUE;  // Non-zero return prevents default processing
}

/*============================================================================*/
    LRESULT CHyperlink::
WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                            /*

*-----------------------------------------------------------------------------*/
{
    try
    {
        switch (msg)
        {
            case WM_LBUTTONDOWN:
                return OnLButtonDown(msg, wparam, lparam);

            case WM_LBUTTONUP:
                return OnLButtonUp(msg, wparam, lparam);

            case WM_SETCURSOR:
                return OnSetCursor(msg, wparam, lparam);

            case WM_NCHITTEST:
                return HTCLIENT;// Claim that the mouse is in a client area
        }
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