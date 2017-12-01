/* [Tab/Indent: 8/8][Line/Box: 80/74]                          (Hyperlink.cpp) *
********************************************************************************

    Contents Description: Implementation of the CHyperlink class for a 
    sample program demonstrating the extended use of the status bar using the 
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
#include "Hyperlink.h"

/*============================================================================*/
    CHyperlink::
CHyperlink()                                /*

    Create the custom hyperlink for this sample program.
*-----------------------------------------------------------------------------*/
    :   m_IsUrlVisited(FALSE), m_IsClicked(FALSE), 
        m_crVisited(HYTextColor[0]), m_crNotVisited(HYTextColor[1])
{
      // Create the cursor
    m_hCursor = ::LoadCursor(NULL, IDC_HAND);

      // IDC_HAND is not available on Win95, so load a reasonable 
      // alternative
    if( !m_hCursor )
        m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
}

/*============================================================================*/
    CHyperlink::
~CHyperlink()                               /* 

*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    void CHyperlink::
OnAttach()                              /*

    Set the content of the hyperlink status bar part and modify the font
    to underscore the part string.
*-----------------------------------------------------------------------------*/
{
    SetWindowText(_T("Win32++"));
    NONCLIENTMETRICS ncm = GetNonClientMetrics();
    LOGFONT lf = ncm.lfMessageFont;
    lf.lfUnderline = TRUE;
    m_UrlFont.CreateFontIndirect(lf);
}

/*============================================================================*/
    LRESULT CHyperlink::
OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)          /*

    Handle the left mouse button has been pressed down.
*-----------------------------------------------------------------------------*/
{
    SetCapture();
    m_IsClicked = TRUE;
    return FinalWindowProc(uMsg, wParam, lParam);
}

/*============================================================================*/
    LRESULT CHyperlink::
OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)            /*

    Handle the release of the mouse button after pressing down.
*-----------------------------------------------------------------------------*/
{
    ReleaseCapture();
    if(m_IsClicked)
    {
        m_IsClicked = FALSE;
        CPoint pt;
        pt.x = (short)LOWORD(lParam);
        pt.y = (short)HIWORD(lParam);
        ClientToScreen(pt);
        CRect rc = GetWindowRect(); 
        if (rc.PtInRect(pt)) 
            OpenUrl();
    }
    return FinalWindowProc(uMsg, wParam, lParam);
}

/*============================================================================*/
    void CHyperlink::
OpenUrl()                               /*

    Respond the the selection of the hyperlink text by displaying the
    Win32++ SourceForge page in the browser.
*-----------------------------------------------------------------------------*/
{
    TCHAR szUrl[ MAX_PATH + 1 ] = 
        _T("http://sourceforge.net/projects/win32-framework/");
    
    HINSTANCE result = ::ShellExecute(NULL, _T("open"), szUrl, NULL, NULL,
        SW_SHOWNORMAL);

    if (reinterpret_cast<int>(result) > 32)
    {
        m_IsUrlVisited = TRUE;
          // redraw the StatusBar to update the color
        GetParent().RedrawWindow();
    }
}

/*============================================================================*/
    LRESULT CHyperlink::
OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)       /*

    Reflect WM_CTLCOLORSTATIC messages back to the CWnd object that 
    created them for processing.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lParam);

    if (uMsg ==  WM_CTLCOLORSTATIC)
    {
        CDC dc(reinterpret_cast<HDC>(wParam));
        dc.SolidFill(HYBkgrColor, GetClientRect());
        dc.SetTextColor( m_IsUrlVisited? m_crVisited : m_crNotVisited);
        dc.SetBkMode(TRANSPARENT);
        dc.SelectObject(m_UrlFont);
          // Return a NULL brush for a transparent background
        return (LRESULT)GetStockObject(NULL_BRUSH);
    }
    return 0L;
}

/*============================================================================*/
    LRESULT CHyperlink::
OnSetCursor(UINT, WPARAM, LPARAM)                   /*

*-----------------------------------------------------------------------------*/
{
        // Must use ::SetCursor here. CStatic::SetCursor does not do the 
        // same thing. 
    ::SetCursor(m_hCursor);
    return 1L;  // Non-zero return prevents default processing
}

/*============================================================================*/
    LRESULT CHyperlink::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)            /*

*-----------------------------------------------------------------------------*/
{
    switch (uMsg)
    {
        case WM_LBUTTONDOWN: 
        return OnLButtonDown(uMsg, wParam, lParam);

        case WM_LBUTTONUP:    
        return OnLButtonUp(uMsg, wParam, lParam);

        case WM_SETCURSOR:    
        return OnSetCursor(uMsg, wParam, lParam);   

        case WM_NCHITTEST:    
        return HTCLIENT;// Claim that the mouse is in a client area
    }
    return WndProcDefault(uMsg, wParam, lParam);
}
