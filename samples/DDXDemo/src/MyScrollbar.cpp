/////////////////////////////////////////
// MyScrollbar.cpp
// Authors: Robert Tausworthe, David Nash
//


#include "pch.h"
#include "MyScrollbar.h"
#include "App.h"
#include "resource.h"

/////////////////////////////////////
// CMyScrollBar function definitions.
//

CMyScrollBar::CMyScrollBar()
{
        m_scrollInfo = {};
}

// Respond to WM_HSCROLL messages and return the current position of the
// scroll bar.
int CMyScrollBar::GetHScrollPos(WPARAM wparam, LPARAM)
{
    GetScrollInfo(m_scrollInfo);
    switch (LOWORD (wparam))
    {
        case SB_LINELEFT:  // User clicked left or up arrow.
        m_scrollInfo.nPos -= 1;
        break;

        case SB_LINERIGHT:  // User clicked right or down arrow.
        m_scrollInfo.nPos += 1;
        break;

        case SB_PAGELEFT:  // User clicked page up or scroll bar left of thumb.
        m_scrollInfo.nPos -= m_scrollInfo.nPage;
        break;

        case SB_PAGERIGHT: // User clicked page down or scroll bar right of thumb.
        m_scrollInfo.nPos += m_scrollInfo.nPage;
        break;

        case SB_THUMBTRACK: // User dragged the scroll box.
        m_scrollInfo.nPos = m_scrollInfo.nTrackPos;
        break;

        default: break;
    }

    return m_scrollInfo.nPos;
}

void CMyScrollBar::SetScrollInfo(int lo, int hi, int pos, int page)
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
