//////////////////////////////////////////////
// ColourDialog.cpp

#include "stdafx.h"
#include "FastGDIApp.h"
#include "Mainfrm.h"
#include "ColourDialog.h"
#include "resource.h"


CColourDialog::CColourDialog(UINT nResID) : CDialog(nResID)
{
	m_cRed = 0;
	m_cGreen = 0;
	m_cBlue = 0;
}

CColourDialog::~CColourDialog()
{
}

BOOL CColourDialog::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (LOWORD(wParam))
	{
	case IDC_CHECK1:
		OnGrayScale();
		return TRUE;	// return TRUE for handled commands
	}

	// return FALSE for unhandled commands
	return FALSE;
}

void CColourDialog::OnGrayScale()
{
	// Update the colour of the preview image
	if (SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0))
		GrayScaleBitmap(&m_bmPreview);
	else
	{
		// Copy m_hbmPreviewOrig to m_hbmPreview
		CMemDC Mem1DC(NULL);
		CBitmap* pOldBitmap1 = Mem1DC.SelectObject(&m_bmPreviewOrig);
		CMemDC Mem2DC(NULL);
		CBitmap* pOldBitmap2 = Mem2DC.SelectObject(&m_bmPreview);
		Mem2DC.BitBlt(0, 0, 239-16, 201-16, &Mem1DC, 0, 0, SRCCOPY);
		Mem1DC.SelectObject(pOldBitmap1);
		Mem2DC.SelectObject(pOldBitmap2);

		TintBitmap(&m_bmPreview, m_cRed, m_cGreen, m_cBlue);
	}

	OnPaintPreview();
}

BOOL CColourDialog::OnInitDialog()
{
	// Attach the Trackbar controls to CWnd objects
	m_RedSlider.AttachDlgItem(IDC_SLIDER_RED, this);
	m_GreenSlider.AttachDlgItem(IDC_SLIDER_GREEN, this);
	m_BlueSlider.AttachDlgItem(IDC_SLIDER_BLUE, this);

	// Set Trackbar ranges
	m_RedSlider.SendMessage(TBM_SETRANGE,  (WPARAM)TRUE, (LPARAM)MAKELONG(-255, 255));
	m_GreenSlider.SendMessage(TBM_SETRANGE,  (WPARAM)TRUE, (LPARAM)MAKELONG(-255, 255));
	m_BlueSlider.SendMessage(TBM_SETRANGE,  (WPARAM)TRUE, (LPARAM)MAKELONG(-255, 255));

	// Attach the Edit controls to CWnd objects
	m_RedEdit.AttachDlgItem(IDC_EDIT_RED, this);
	m_GreenEdit.AttachDlgItem(IDC_EDIT_GREEN, this);
	m_BlueEdit.AttachDlgItem(IDC_EDIT_BLUE, this);

	m_RedEdit.SendMessage(WM_SETTEXT, 0, (LPARAM)_T("0"));
	m_GreenEdit.SendMessage(WM_SETTEXT, 0, (LPARAM)_T("0"));
	m_BlueEdit.SendMessage(WM_SETTEXT, 0, (LPARAM)_T("0"));

	// Create the two image previews
	m_Preview.AttachDlgItem(IDC_PREVIEW, this);
	CreateImagePreviews(GetFrameApp().GetMainFrame().GetMyView().GetImage());

	return TRUE;
}

INT_PTR CColourDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_HSCROLL:
		OnHScroll(wParam, lParam);
		break;

	case WM_PAINT:
		OnPaintPreview();
		return 0;
	}

	return DialogProcDefault(uMsg, wParam, lParam);
}

void CColourDialog::OnHScroll(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	// Update the text for the colour's edit control
	int nPos = SendMessage((HWND)lParam, TBM_GETPOS, 0, 0);
	TCHAR Text[5];
	wsprintf(Text, _T("%d\0"), nPos);

	if ((HWND)lParam == m_RedSlider.GetHwnd())
		SendMessage(m_RedEdit.GetHwnd(), WM_SETTEXT, 0, (LPARAM)&Text);
	else if ((HWND)lParam == m_GreenSlider.GetHwnd())
		SendMessage(m_GreenEdit.GetHwnd(), WM_SETTEXT, 0, (LPARAM)&Text);
	else if ((HWND)lParam == m_BlueSlider.GetHwnd())
		SendMessage(m_BlueEdit.GetHwnd(), WM_SETTEXT, 0, (LPARAM)&Text);

	// Store the colour values
	m_cRed   = m_RedSlider.SendMessage(TBM_GETPOS);
	m_cGreen = m_GreenSlider.SendMessage(TBM_GETPOS);
	m_cBlue  = m_BlueSlider.SendMessage(TBM_GETPOS);

	// Copy m_hbmPreviewOrig to m_hbmPreview
	CMemDC Mem1DC(NULL);
	CBitmap* pBitmapOld1 = Mem1DC.SelectObject(&m_bmPreviewOrig);
	CMemDC Mem2DC(NULL);
	CBitmap* pBitmapOld2 = Mem2DC.SelectObject(&m_bmPreview);
	Mem2DC.BitBlt(0, 0, 239-16, 201-16, &Mem1DC, 0, 0, SRCCOPY);
	Mem1DC.SelectObject(pBitmapOld1);
	Mem2DC.SelectObject(pBitmapOld2);

	// Update the colour of the preview image
	TintBitmap(&m_bmPreview, m_cRed, m_cGreen, m_cBlue);

	if (SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0))
		GrayScaleBitmap(&m_bmPreview);

	OnPaintPreview();
}

void CColourDialog::OnOK()
{
	// Get a reference to our CMainFrame object
	CMainFrame& MainFrame = GetFrameApp().GetMainFrame();
	BOOL bGray = SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0);
	MainFrame.ModifyBitmap(m_cRed, m_cGreen, m_cBlue, bGray);

	CDialog::OnOK();
}

void CColourDialog::OnPaintPreview()
// Displays the bitmap in the display area of our dialog
{
	BITMAP bm;
	::GetObject(m_bmPreview, sizeof(BITMAP), &bm);

	// Get the size of the destination display area
	CRect rcView = m_Preview.GetClientRect();
	::MapWindowPoints(m_Preview.GetHwnd(), m_hWnd, (LPPOINT)&rcView, 2);

	int nLeftDest;
	int nTopDest;
	if (bm.bmWidth < bm.bmHeight*rcView.Width()/rcView.Height())
	{
		nLeftDest = rcView.left + (rcView.Width() - bm.bmWidth)/2;
		nTopDest = rcView.top;
	}
	else
	{
		nLeftDest = rcView.left;
		nTopDest = rcView.top + (rcView.Height() - bm.bmHeight)/2;
	}

	CClientDC dcPreview(this);
	CMemDC MemDC(&dcPreview);
	CBitmap* pBitmapOld = MemDC.SelectObject(&m_bmPreview);
	dcPreview.BitBlt(nLeftDest, nTopDest, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pBitmapOld);
}

void CColourDialog::CreateImagePreviews(HBITMAP hbmImage)
// Creates the two Preview bitmaps: m_Preview and m_PreviewOrig
{
	// Get the size of the bitmap
	BITMAP bm;
	::GetObject(hbmImage, sizeof(BITMAP), &bm);

	// Get the size of the destination display area
	CRect rcView = m_Preview.GetClientRect();
	m_Preview.MapWindowPoints(this, (LPPOINT)&rcView, 2);

	// Calculate the stretch values, preserving the aspect ratio
	int nWidthDest;
	int nHeightDest;
	double AspectRatio;

	if (bm.bmWidth < bm.bmHeight*rcView.Width()/rcView.Height())
	{
		AspectRatio = (double)bm.bmWidth / (double)bm.bmHeight;
		nWidthDest = (int)(rcView.Height()*AspectRatio);
		nHeightDest = rcView.Height();
	}
	else
	{
		AspectRatio = (double)bm.bmHeight / (double)bm.bmWidth;
		nWidthDest = rcView.Width();
		nHeightDest = (int)(rcView.Width()*AspectRatio);
	}

	// Create the Device Contexts and compatible bitmaps
	CMemDC Dest1DC(NULL);
	CMemDC Dest2DC(NULL);
	CMemDC MemDC(NULL);
	CClientDC dcDesktop(NULL);
	m_bmPreview.CreateCompatibleBitmap(&dcDesktop, nWidthDest, nHeightDest);
	m_bmPreviewOrig.CreateCompatibleBitmap(&dcDesktop, nWidthDest, nHeightDest);
	MemDC.SelectObject(FromHandle(hbmImage));
	Dest1DC.SelectObject(&m_bmPreview);
	Dest2DC.SelectObject(&m_bmPreviewOrig);

	// Stretch the bitmap to fit in the destination display area
	Dest1DC.SetStretchBltMode(COLORONCOLOR);
	Dest1DC.StretchBlt(0, 0, nWidthDest, nHeightDest, &MemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	// Make a second copy of the bitmap
	Dest2DC.BitBlt(0, 0, nWidthDest, nHeightDest, &Dest1DC, 0, 0, SRCCOPY);
}

