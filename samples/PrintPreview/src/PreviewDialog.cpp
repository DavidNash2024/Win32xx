///////////////////////////////////////
// PreviewDialog.cpp

#include "stdafx.h"
#include "PreviewDialog.h"
#include "TextApp.h"
#include "resource.h"


/////////////////////////////////////////
// Definitions for the CPreviewPane class
//
CPreviewPane::CPreviewPane()
{
	// The entry for the dialog's control in resource.rc must match this name.
	CString ClassName = _T("PreviewPane");

	// Register the window class for use as a custom control in the dialog
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));

	if (!::GetClassInfo(GetApp().GetInstanceHandle(), ClassName, &wc))
	{
		wc.lpszClassName = ClassName;
		wc.lpfnWndProc = ::DefWindowProc;
		wc.hInstance = GetApp().GetInstanceHandle();
		wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		::RegisterClass(&wc);
	}

	assert(::GetClassInfo(GetApp().GetInstanceHandle(), ClassName, &wc));
}

BOOL CPreviewPane::OnEraseBkgnd(CDC& )
{
	// prevent default erasure of the background
	return TRUE; 
}


void CPreviewPane::OnDraw(CDC& dc)
{
	Render(dc);
}

LRESULT CPreviewPane::OnPaint(UINT, WPARAM, LPARAM)
// Normally OnDraw is suppressed for controls, but we need OnDraw for this window.
{
	if (::GetUpdateRect(*this, NULL, FALSE))
	{
		CPaintDC dc(*this);
		OnDraw(dc);
	}
	else
	// RedrawWindow can require repainting without an update rect
	{
		CClientDC dc(*this);
		OnDraw(dc);
	}

	// No more drawing required
	return 0L;
}

void CPreviewPane::Render(CDC& dc)
// Copies the bitmap (m_Bitmap) to the PreviewPane, scaling the image 
//  to fit the window.
{
	if (m_Bitmap.GetHandle())
	{
		CMemDC dcMem(dc);
		dcMem.SelectObject(m_Bitmap);
		BITMAP bm = m_Bitmap.GetBitmapData();
		int Border = 20;
		CRect rcClient = GetClientRect();

		double ratio = double(bm.bmHeight) / double(bm.bmWidth);
		int PreviewWidth;
		int PreviewHeight;
		
		// These borders center the preview with the PreviewPane.
		int xBorder = Border;
		int yBorder = Border;

		if ((rcClient.Width() - 2*Border)*ratio < (rcClient.Height() - 2*Border))
		{
			PreviewWidth = rcClient.Width() - (2*Border);
			PreviewHeight = (int)(PreviewWidth * ratio);

			yBorder = (rcClient.Height() - PreviewHeight) / 2;
		}
		else
		{
			PreviewHeight = rcClient.Height() - (2*Border);
			PreviewWidth = (int)(PreviewHeight / ratio);

			xBorder = (rcClient.Width() - PreviewWidth) / 2;
		}

		// Copy from the memory dc to the PreviewPane's DC with stretching.
		dc.StretchBlt(xBorder, yBorder, PreviewWidth, PreviewHeight, dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	
		// Draw a grey border around the preview
		// OnEraseBkgnd suppresses background drawing to avoid flicker on Windows XP and earlier 
		CRect rcFill(0, 0, xBorder, PreviewHeight + yBorder);
		dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));

		rcFill.SetRect(0, 0, PreviewWidth + xBorder, yBorder);
		dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));

		rcFill.SetRect(PreviewWidth + xBorder, 0, rcClient.Width(), rcClient.Height());
		dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));

		rcFill.SetRect(0, PreviewHeight + yBorder, rcClient.Width(), rcClient.Height());
		dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));
	}
}

///////////////////////////////////////////
// Definitions for the CPreviewDialog class
//
CPreviewDialog::CPreviewDialog(UINT nResID) : CDialog(nResID), m_CurrentPage(0)
{
}

CPreviewDialog::~CPreviewDialog()
{
}

CRichEdit& CPreviewDialog::GetRichView()
{
	CMainFrame& MainFrame = GetTextApp().GetMainFrame();
	return MainFrame.GetRichView();
}

INT_PTR CPreviewDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Pass resizing messages on to the resizer
	m_Resizer.HandleMessage(uMsg, wParam, lParam);

//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL CPreviewDialog::OnCloseButton()
{
	GetAncestor().SendMessage(UWM_CHANGEVIEW);
	return TRUE;
}

BOOL CPreviewDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	UINT nID = LOWORD(wParam);
	switch (nID)
    {
	case IDC_BUTTONPRINT:	return OnPrintButton();
	case IDC_BUTTONPREV:	return OnPrevButton();
	case IDC_BUTTONNEXT:	return OnNextButton();
	case IDC_BUTTONCLOSE:	return OnCloseButton();
    }

	return FALSE;
}

BOOL CPreviewDialog::OnInitDialog()
{
	// Attach the dialog controls to CWnd objects
	AttachItem(IDC_BUTTONPRINT, m_ButtonPrint);
	AttachItem(IDC_BUTTONPREV, m_ButtonPrev);
	AttachItem(IDC_BUTTONNEXT, m_ButtonNext);
	AttachItem(IDC_BUTTONCLOSE, m_ButtonClose);
	AttachItem(IDC_CUSTOM1, m_PreviewPane);

	// Support dialog resizing
	m_Resizer.Initialize(*this, CRect(0, 0, 100, 120));
	m_Resizer.AddChild(m_ButtonPrint, topleft, 0);
	m_Resizer.AddChild(m_ButtonPrev, topleft, 0);
	m_Resizer.AddChild(m_ButtonNext, topleft, 0);
	m_Resizer.AddChild(m_ButtonClose, topleft, 0);
	m_Resizer.AddChild(m_PreviewPane, topleft, RD_STRETCH_WIDTH | RD_STRETCH_HEIGHT);

	m_CurrentPage = 0;
	return true;
}

BOOL CPreviewDialog::OnNextButton()
{
	PreviewPage(++m_CurrentPage);
	UpdateButtons();

	return TRUE;
}

BOOL CPreviewDialog::OnPrevButton()
{
	PreviewPage(--m_CurrentPage);
	UpdateButtons();
	
	return TRUE;
}

BOOL CPreviewDialog::OnPrintButton()
{
	GetAncestor().SendMessage(UWM_PRINTNOW);
	return TRUE; 
}

BOOL CPreviewDialog::DoPrintPreview(RECT rcPage, RECT rcPrintArea)
{
	m_rcPage = rcPage;
	m_rcPrintArea = rcPrintArea;

	// Get the device contect of the default or currently chosen printer
	CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
	CDC dcPrinter = PrintDlg.GetPrinterDC();
	dcPrinter.SetMapMode(MM_TEXT);

	FORMATRANGE fr;
	fr.hdcTarget = dcPrinter;
	fr.hdc = dcPrinter;
	fr.rcPage = rcPage;
	fr.rc = rcPrintArea;
	fr.chrg.cpMin = 0;
	fr.chrg.cpMax = -1;

	// Find out real size of document in characters.
	LONG lTextLength;   // Length of document.
	lTextLength = GetRichView().GetTextLengthEx(GTL_NUMCHARS);

	// Calculate the page breaks
	LONG lTextPrinted;  // Amount of document printed.
	m_PageBreaks.clear();
	do
	{
		lTextPrinted = GetRichView().FormatRange(fr, FALSE);

		if (lTextPrinted < lTextLength)
		{
			fr.chrg.cpMin = lTextPrinted;
			fr.chrg.cpMax = -1;
			m_PageBreaks.push_back(lTextPrinted);
		}
	} while (lTextPrinted < lTextLength);

	m_PageBreaks.push_back(-1);

	// Clear the cache
	GetRichView().FormatRange();

	// Preview the first page;
	m_CurrentPage = 0;
	PreviewPage(0);

	return TRUE;
}

void CPreviewDialog::PreviewPage(UINT nPage)
{
	assert(m_PageBreaks.size() > 0);
	assert(nPage < m_PageBreaks.size());

	// Get the device contect of the default or currently chosen printer
	CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
	CDC dcPrinter = PrintDlg.GetPrinterDC();
	
	// Create a memory DC for the preview pane
	CDC dcPreview = GetPreviewPane().GetDC(); 
	CMemDC dcMem(dcPreview);

	// Assign values to the FORMATRANGE struct
	FORMATRANGE fr;
	ZeroMemory(&fr, sizeof(fr));
	fr.hdcTarget = dcPrinter;
	fr.hdc = dcMem;
	fr.rcPage = m_rcPage;
	fr.rc = m_rcPrintArea;
	fr.chrg.cpMin = (nPage > 0) ? m_PageBreaks[nPage - 1] : 0;
	fr.chrg.cpMax = m_PageBreaks[nPage];

	// Create a compatible bitmap for the memory DC
	double rescale = 1.15;
	int Width = (int)(rescale * GetDeviceCaps(dcPreview, LOGPIXELSX) * (dcPrinter.GetDeviceCaps(HORZRES) / dcPrinter.GetDeviceCaps(LOGPIXELSX)));
	int Height = (int)(rescale * GetDeviceCaps(dcPreview, LOGPIXELSY) * (dcPrinter.GetDeviceCaps(VERTRES) / dcPrinter.GetDeviceCaps(LOGPIXELSY)));

	dcMem.CreateCompatibleBitmap(dcPreview, Width, Height);
	CRect rc(0, 0, Width, Height);
	dcMem.FillRect(rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));

	// Display text from the richedit control on the memory dc
	dcMem.SetMapMode(MM_TEXT);
	GetRichView().FormatRange(fr, TRUE);

	// Tell the control to release the cached information.
	GetRichView().FormatRange();

	// Save the bitmap from the memory DC
	CBitmap Bitmap = dcMem.DetachBitmap();
	GetPreviewPane().SetBitmap(Bitmap);

	// Display the print preview
	UpdateButtons();
	GetPreviewPane().Render(dcPreview);
}

void CPreviewDialog::UpdateButtons()
{
	m_ButtonNext.EnableWindow(m_CurrentPage < m_PageBreaks.size() - 1);
	m_ButtonPrev.EnableWindow(m_CurrentPage > 0);
}

