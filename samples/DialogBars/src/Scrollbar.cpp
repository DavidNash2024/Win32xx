//////////////////////////////////////////////
// CMyScrollBar.cpp

#include "stdafx.h"
#include "ScrollBar.h"
#include "DialogApp.h"


CMyScrollBar::CMyScrollBar()
{
	ZeroMemory(&m_si, sizeof(SCROLLINFO));
}

void CMyScrollBar::OnInitialUpdate()
{
	// Called automatically via AttachItem

	// Set the Scroll bar position
	SetScroll(0);
}

LRESULT CMyScrollBar::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(lParam);

	// Get a pointer to the MyDialog object
	CMyDialog& MyDialog = GetDialogApp().GetDialog();

	GetScrollInfo(m_si);

	switch (LOWORD (wParam))
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

	MyDialog.SetScroll(m_si.nPos);			// Set the scroll bar position
	MyDialog.SetSlider(m_si.nPos);			// Set the slider position
	MyDialog.SetProgress(m_si.nPos);		 // Set the progress bar position
	MyDialog.SetStatic(FALSE, m_si.nPos);	 // Set the static text

	return 0L;
}

LRESULT CMyScrollBar::OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (uMsg)
	{
	case WM_HSCROLL:	return OnHScroll(uMsg, wParam, lParam);
	}

	return 0L;
}

void CMyScrollBar::SetScroll(int nPos)
{
	m_si.cbSize = sizeof(SCROLLINFO);
	m_si.nPos = nPos;
	m_si.nPage = 10;
	m_si.nMax = 100 + (m_si.nPage -1);
	m_si.fMask = SIF_ALL;

	// Set the scroll bar position
	SetScrollInfo(m_si, TRUE);
}

