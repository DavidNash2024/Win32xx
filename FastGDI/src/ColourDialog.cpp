


#include "stdafx.h"
#include "FastGDIApp.h"
#include "Mainfrm.h"
#include "ColourDialog.h"
#include "resource.h"


CColourDialog::CColourDialog(UINT nResID) : CDialog(nResID), m_hbmPreview(0), m_hbmPreviewOrig(0)
{
	m_cRed = 0;
	m_cGreen = 0;
	m_cBlue = 0;
}

CColourDialog::~CColourDialog()
{
	if (m_hbmPreview) ::DeleteObject(m_hbmPreview);
	if (m_hbmPreviewOrig) ::DeleteObject(m_hbmPreviewOrig);
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
		GrayScaleBitmap(m_hbmPreview);
	else
	{
		// Copy m_hbmPreviewOrig to m_hbmPreview
		CDC Mem1DC = ::CreateCompatibleDC(NULL);
		Mem1DC.AttachBitmap(m_hbmPreviewOrig);
		CDC Mem2DC = ::CreateCompatibleDC(NULL);
		Mem2DC.AttachBitmap(m_hbmPreview);
		Mem2DC.BitBlt(0, 0, 239-16, 201-16, Mem1DC, 0, 0, SRCCOPY);
		Mem1DC.DetachBitmap();
		Mem2DC.DetachBitmap();

		TintBitmap(m_hbmPreview, m_cRed, m_cGreen, m_cBlue);
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

BOOL CColourDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void CColourDialog::OnHScroll(WPARAM /*wParam*/, LPARAM lParam)
{
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
	CDC Mem1DC = ::CreateCompatibleDC(NULL);
	Mem1DC.AttachBitmap(m_hbmPreviewOrig);
	CDC Mem2DC = ::CreateCompatibleDC(NULL);
	Mem2DC.AttachBitmap(m_hbmPreview);
	Mem2DC.BitBlt(0, 0, 239-16, 201-16, Mem1DC, 0, 0, SRCCOPY);
	Mem1DC.DetachBitmap();
	Mem2DC.DetachBitmap();

	// Update the colour of the preview image
	TintBitmap(m_hbmPreview, m_cRed, m_cGreen, m_cBlue);

	if (SendDlgItemMessage(IDC_CHECK1, BM_GETCHECK, 0, 0))
		GrayScaleBitmap(m_hbmPreview);

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
	::GetObject(m_hbmPreview, sizeof(BITMAP), &bm);

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

	CDC PreviewDC = GetDC();
	CDC MemDC = PreviewDC.CreateCompatibleDC();
	MemDC.AttachBitmap(m_hbmPreview);
	PreviewDC.BitBlt(nLeftDest, nTopDest, bm.bmWidth, bm.bmHeight, MemDC, 0, 0, SRCCOPY);
	MemDC.DetachBitmap();
}

void CColourDialog::CreateImagePreviews(HBITMAP hbmImage)
// Creates the two Preview bitmaps: m_Preview and m_PreviewOrig
{
	// Get the size of the bitmap
	BITMAP bm;
	::GetObject(hbmImage, sizeof(BITMAP), &bm);

	// Get the size of the destination display area
	CRect rcView = m_Preview.GetClientRect();
	::MapWindowPoints(m_Preview.GetHwnd(), m_hWnd, (LPPOINT)&rcView, 2);

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
	CDC Dest1DC = ::CreateCompatibleDC(NULL);
	CDC Dest2DC = ::CreateCompatibleDC(NULL);
	CDC MemDC  = ::CreateCompatibleDC(NULL);
	CDC DesktopDC = ::GetDC(NULL);
	if (m_hbmPreview) 	::DeleteObject(m_hbmPreview);
	if (m_hbmPreviewOrig) :: DeleteObject(m_hbmPreviewOrig);
	m_hbmPreview = ::CreateCompatibleBitmap(DesktopDC, nWidthDest, nHeightDest);
	m_hbmPreviewOrig = ::CreateCompatibleBitmap(DesktopDC, nWidthDest, nHeightDest);
	MemDC.AttachBitmap(hbmImage);
	Dest1DC.AttachBitmap(m_hbmPreview);
	Dest2DC.AttachBitmap(m_hbmPreviewOrig);

	// Stretch the bitmap to fit in the destination display area
	Dest1DC.SetStretchBltMode(COLORONCOLOR);
	Dest1DC.StretchBlt(0, 0, nWidthDest, nHeightDest, MemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	// Make a second copy of the bitmap
	Dest2DC.BitBlt(0, 0, nWidthDest, nHeightDest, Dest1DC, 0, 0, SRCCOPY);

	// Release the bitmaps
	MemDC.DetachBitmap();
	Dest1DC.DetachBitmap();
	Dest2DC.DetachBitmap();
}

