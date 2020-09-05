/////////////////////////////
// ScrollBar.cpp
//

#include "stdafx.h"
#include "ScrollBar.h"
#include "DialogApp.h"

////////////////////////////////////
// CMyScrollBar function definitions
//

// Constructor.
CMyScrollBar::CMyScrollBar()
{
    ZeroMemory(&m_si, sizeof(SCROLLINFO));
}

// Returns the current scroll bar position.
int CMyScrollBar::GetPos(WPARAM wparam)
{
    GetScrollInfo(m_si);

    switch (LOWORD(wparam))
    {
        // user clicked left arrow
    case SB_LINELEFT:
        m_si.nPos -= 1;
        break;

        // user clicked right arrow
    case SB_LINERIGHT:
        m_si.nPos += 1;
        break;

        // user clicked the scroll bar shaft left of the scroll box
    case SB_PAGELEFT:
        m_si.nPos -= m_si.nPage;
        break;

        // user clicked the scroll bar shaft right of the scroll box
    case SB_PAGERIGHT:
        m_si.nPos += m_si.nPage;
        break;

        // user dragged the scroll box
    case SB_THUMBTRACK:
        m_si.nPos = m_si.nTrackPos;
        break;

    default:
        break;
    }

    m_si.nPos = MIN(m_si.nPos, 100);
    m_si.nPos = MAX(m_si.nPos, 0);
    return m_si.nPos;
}

// Called after the Scrollbar is attached.
void CMyScrollBar::OnInitialUpdate()
{
    // Set the Scroll bar position
    SetPos(0);
}

// Sets the scroll bar's position.
void CMyScrollBar::SetPos(int pos)
{
    m_si.cbSize = sizeof(SCROLLINFO);
    m_si.nPos = pos;
    m_si.nPage = 10;
    m_si.nMax = 100 + (m_si.nPage -1);
    m_si.fMask = SIF_ALL;

    // Set the scroll bar position
    SetScrollInfo(m_si, TRUE);
}

