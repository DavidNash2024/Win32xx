///////////////////////////////////////
// MyDialog.cpp

#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, HWND hWndParent)
	: CDialog(nResID, hWndParent)
{
}

CMyDialog::~CMyDialog()
{
}

BOOL CMyDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_HSCROLL:
		OnHScroll(wParam, lParam);
		break;		
	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

void CMyDialog::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	if (GetDlgItem(IDC_SLIDER1) == (HWND)lParam)
	{
		// Slider input

		// Get the slider bar position
		int nPos = ::SendMessage(GetDlgItem(IDC_SLIDER1), TBM_GETPOS, 0, 0);

		SetProgress(nPos);		// Set the progress bar position
		SetScroll(nPos);		// Set the scroll bar position
		SetStatic(TRUE, nPos);	// Set the static text
	}
	
	else if (GetDlgItem(IDC_SCROLLBAR1) == (HWND)lParam)
	{
		// Scrollbar input

		::GetScrollInfo(GetDlgItem(IDC_SCROLLBAR1), SB_CTL, &m_si);
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

		// Set the scroll bar position
		SetScroll(m_si.nPos);
		::GetScrollInfo(GetDlgItem(IDC_SCROLLBAR1), SB_CTL, &m_si);
	
		// Set the slider bar position
		::SendMessage(GetDlgItem(IDC_SLIDER1), TBM_SETPOS, TRUE, (LPARAM)m_si.nPos);
				
		SetProgress(m_si.nPos);			// Set the progress bar position	
		SetStatic(FALSE, m_si.nPos);	// Set the static text
	}
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// Set the Scroll bar settings
	ZeroMemory(&m_si, sizeof(SCROLLINFO));
	m_si.cbSize = sizeof(SCROLLINFO);
	m_si.nPage = 10;
	m_si.nMax = 100 + (m_si.nPage -1);
	m_si.fMask = SIF_ALL;
	::SetScrollInfo(GetDlgItem(IDC_SCROLLBAR1), SB_CTL, &m_si, TRUE);

	// The slider has a tic mark every ten units
	::SendMessage(GetDlgItem(IDC_SLIDER1), TBM_SETTICFREQ,  (WPARAM)10, 0); 

	return true;
}

void CMyDialog::OnOK()
{
	::MessageBox(NULL, _T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
	CDialog::OnOK();
}

void CMyDialog::SetProgress(int nPos)
{
	// Set the progress bar position
	::SendMessage(GetDlgItem(IDC_PROGRESS1), PBM_SETPOS, (WPARAM)nPos, 0);
}

void CMyDialog::SetScroll(int nPos)
{		
	SCROLLINFO si = {0};
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	si.nPos = nPos;
	
	// Set the scroll bar position
	::SetScrollInfo(GetDlgItem(IDC_SCROLLBAR1), SB_CTL, &si, TRUE);
}

void CMyDialog::SetStatic(BOOL IsSlider, int nPos)
{
	TCHAR szStatic[80];
	if (IsSlider)
		wsprintf(szStatic, _T("Slider Position %d"), nPos);
	else
		wsprintf(szStatic, _T("Scroll Position %d"), nPos);

	// Set the text in the static control
	::SetDlgItemText(m_hWnd, IDC_STATIC2, szStatic);
}

