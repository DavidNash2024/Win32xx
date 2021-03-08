/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]          (MyScrollBar.cpp) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The custom scroll bar class implementation for
    this DDX-DDV Test program. This class is derived from the MyScrollBar
    class found in the Win32++ Windows sample programs.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#include "stdafx.h"
#include "MyScrollbar.h"
#include "App.h"
#include "resource.h"

/*============================================================================*/
    CMyScrollBar::
CMyScrollBar()                                                              /*

*-----------------------------------------------------------------------------*/
{
    ZeroMemory(&m_scrollInfo, sizeof(m_scrollInfo));
}

/*============================================================================*/
    int CMyScrollBar::
GetHScrollPos(UINT msg, WPARAM wparam, LPARAM lparam)                       /*

    Respond to WM_HSCROLL messages and return the current position of the
    scroll bar.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);

    GetScrollInfo(m_scrollInfo);
    switch (LOWORD (wparam))
    {
        case SB_LINELEFT:  // user clicked left or up arrow
        m_scrollInfo.nPos -= 1;
        break;

        case SB_LINERIGHT:  // clicked right or down arrow
        m_scrollInfo.nPos += 1;
        break;

        case SB_PAGELEFT:  // clicked page up or scroll bar left of thumb
        m_scrollInfo.nPos -= m_scrollInfo.nPage;
        break;

        case SB_PAGERIGHT: // clicked page down or scroll bar right of thumb
        m_scrollInfo.nPos += m_scrollInfo.nPage;
        break;

        case SB_THUMBTRACK: // user dragged the scroll box
        m_scrollInfo.nPos = m_scrollInfo.nTrackPos;
        break;

        default :
        break;
    }
    return m_scrollInfo.nPos;
}

/*============================================================================*/
    void CMyScrollBar::
SetScrollInfo(int lo, int hi, int pos, int page)                            /*

*-----------------------------------------------------------------------------*/
{
    m_scrollInfo.cbSize = sizeof(SCROLLINFO);
    m_scrollInfo.nPos   = pos;
    m_scrollInfo.nPage  = page;
    m_scrollInfo.nMin   = lo;
    m_scrollInfo.nMax   = hi + (page - 1);
    m_scrollInfo.fMask  = SIF_ALL;

      // Set the scroll bar position
    CScrollBar::SetScrollInfo(m_scrollInfo, TRUE);
}
/*----------------------------------------------------------------------------*/
