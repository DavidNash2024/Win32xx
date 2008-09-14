
#include "../Win32++/gdi.h"
#include "ColourDialog.h"

CColourDialog::~CColourDialog()
{
}

BOOL CColourDialog::OnInitDialog()
{
	// Attach the Trackbar controls to CWnd objects
	m_RedSlider.AttachDlgItem(IDC_SLIDER_RED, this);
	m_GreenSlider.AttachDlgItem(IDC_SLIDER_GREEN, this);
	m_BlueSlider.AttachDlgItem(IDC_SLIDER_BLUE, this);
	
	// Set Trackbar ranges
	m_RedSlider.SendMessage(TBM_SETRANGE,  (WPARAM)TRUE, (LPARAM)MAKELONG(-255, 255));
	m_GreenSlider.SendMessage(TBM_SETRANGE,  (WPARAM)FALSE, (LPARAM)MAKELONG(-255, 255));
	m_BlueSlider.SendMessage(TBM_SETRANGE,  (WPARAM)FALSE, (LPARAM)MAKELONG(-255, 255));
	
	// Set tick marks
	m_RedSlider.SendMessage(TBM_SETTICFREQ,  (WPARAM)64, 0);
	m_GreenSlider.SendMessage(TBM_SETTICFREQ,  (WPARAM)64, 0);
	m_BlueSlider.SendMessage(TBM_SETTICFREQ,  (WPARAM)64, 0);

	// Attach the Edit controls to CWnd objects
	m_RedEdit.AttachDlgItem(IDC_EDIT_RED, this);
	m_GreenEdit.AttachDlgItem(IDC_EDIT_GREEN, this);
	m_BlueEdit.AttachDlgItem(IDC_EDIT_BLUE, this);

	m_RedEdit.SendMessage(WM_SETTEXT, 0, (LPARAM)_T("0"));
	m_GreenEdit.SendMessage(WM_SETTEXT, 0, (LPARAM)_T("0"));
	m_BlueEdit.SendMessage(WM_SETTEXT, 0, (LPARAM)_T("0"));

	// Attach the Bitmap window
	m_hWndBitmap.AttachDlgItem(IDC_BITMAP1, this);



	return TRUE;
}

BOOL CColourDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_HSCROLL:
		{
			int nPos = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
			TCHAR Text[5];
			wsprintf(Text, _T("%d\0"), nPos);

			if ((HWND)lParam == m_RedSlider.GetHwnd())
				SendMessage(m_RedEdit.GetHwnd(), WM_SETTEXT, 0, (LPARAM)&Text);
			else if ((HWND)lParam == m_GreenSlider.GetHwnd())
				SendMessage(m_GreenEdit.GetHwnd(), WM_SETTEXT, 0, (LPARAM)&Text);
			else if ((HWND)lParam == m_BlueSlider.GetHwnd())
				SendMessage(m_BlueEdit.GetHwnd(), WM_SETTEXT, 0, (LPARAM)&Text);
		}
		break;

	case WM_PAINT:
		{
			OnPaint();
		}
		return 0;
	}

	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

void CColourDialog::OnPaint()
// Displays the bitmap in the display area of our dialog 
{
	// Get the size of the bitmap
	BITMAP bm;
	::GetObject(m_hBitmap, sizeof(BITMAP), &bm);

	// Get the size of the destination display area
	CRect rcView = m_hWndBitmap.GetClientRect();
	::MapWindowPoints(m_hWndBitmap.GetHwnd(), m_hWnd, (LPPOINT)&rcView, 2);

	// Calculate the stretch values, preserving the aspect ratio
	int nLeftDest;
	int nTopDest;
	int nWidthDest;
	int nHeightDest;
	double AspectRatio;
	
	if (bm.bmWidth < bm.bmHeight)
	{
		AspectRatio = (double)bm.bmWidth / (double)bm.bmHeight;
		nWidthDest = (int)(rcView.Height()*AspectRatio);
		nHeightDest = rcView.Height();
		nLeftDest = rcView.left + (rcView.Width() - nWidthDest)/2;
		nTopDest = rcView.top;
	}
	else
	{
		AspectRatio = (double)bm.bmHeight / (double)bm.bmWidth;
		nWidthDest = rcView.Width();
		nHeightDest = (int)(rcView.Width()*AspectRatio);
		nLeftDest = rcView.left;
		nTopDest = rcView.top + (rcView.Height() - nHeightDest)/2;
	}

	// Copy the bitmap to a memory DC
	CDC ViewDC = GetDC(m_hWnd);
	CDC MemDC = ::CreateCompatibleDC(ViewDC);
	MemDC.AttachBitmap(m_hBitmap);
	
	// Stretch the bitmap to fit in the destination display area
	SetStretchBltMode(ViewDC, COLORONCOLOR);	
	::StretchBlt(ViewDC, nLeftDest, nTopDest, nWidthDest, nHeightDest, MemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	
	// Release the bitmap
	MemDC.DetachBitmap();
}

void CColourDialog::SetBitmap(HBITMAP hbm)
{
	m_hBitmap = hbm;
}

