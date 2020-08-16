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

// Called after the Scrollbar is attached.
void CMyScrollBar::OnInitialUpdate()
{
    // Set the Scroll bar position
    SetScroll(0);
}

// Called when the scrollbar is repositioned.
LRESULT CMyScrollBar::OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);

    // Get a pointer to the MyDialog object
    CMyDialog& myDialog = GetDialogApp()->GetDialog();

    GetScrollInfo(m_si);

    switch (LOWORD (wparam))
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

    default :
        break;
    }

    myDialog.SetScroll(m_si.nPos);          // Set the scroll bar position
    myDialog.SetSlider(m_si.nPos);          // Set the slider position
    myDialog.SetProgress(m_si.nPos);         // Set the progress bar position
    myDialog.SetStatic(FALSE, m_si.nPos);    // Set the static text

    return 0;
}

// Process messages reflected back from the dialog.
LRESULT CMyScrollBar::OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    switch (msg)
    {
    case WM_HSCROLL:    return OnHScroll(msg, wparam, lparam);
    }

    return 0;
}

// Sets the scroll bar's position.
void CMyScrollBar::SetScroll(int pos)
{
    m_si.cbSize = sizeof(SCROLLINFO);
    m_si.nPos = pos;
    m_si.nPage = 10;
    m_si.nMax = 100 + (m_si.nPage -1);
    m_si.fMask = SIF_ALL;

    // Set the scroll bar position
    SetScrollInfo(m_si, TRUE);
}

