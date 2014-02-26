//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class


#include "stdafx.h"
#include "PictureApp.h"
#include "view.h"
#include "resource.h"

#define HIMETRIC_INCH	2540

CView::CView() : m_pPicture(NULL), m_xCurrentScroll(0), m_yCurrentScroll(0)
{
	::CoInitialize(NULL);
}

CView::~CView()
{
	if (m_pPicture)
		m_pPicture->Release();

	::CoUninitialize();
}

CRect CView::GetImageRect()
{
	// get width and height of picture
	long hmWidth = 0;
	long hmHeight = 0;

	if (m_pPicture)
	{
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);
	}

	// convert himetric to pixels
	CClientDC dc(this);
	int nWidth	= MulDiv(hmWidth, dc.GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
	int nHeight	= MulDiv(hmHeight, dc.GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);

	CRect rcImage;
	rcImage.right = MAX(nWidth, 200);
	rcImage.bottom = MAX(nHeight, 200);
	return rcImage;
}

void CView::NewPictureFile()
{
	if (m_pPicture)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
	}

	GetParent()->SetWindowText(LoadString(IDW_MAIN).c_str());
	Invalidate();
}

BOOL CView::LoadPictureFile(LPCTSTR szFile)
{
	if (m_pPicture)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
	}

	TRACE(szFile);
	TRACE("\n");

	m_xCurrentScroll = 0;
	m_yCurrentScroll = 0;
	ShowScrollBar(SB_HORZ, FALSE);
	ShowScrollBar(SB_VERT, FALSE);

	// Create IPicture from image file
	if (S_OK == ::OleLoadPicturePath(T2OLE(szFile), NULL, 0, 0,	IID_IPicture, (LPVOID *)&m_pPicture))
	{
		GetParent()->SendMessageW(UWM_FILELOADED, 0, (LPARAM)szFile);
		Invalidate();
		return TRUE;
	}
	else
	{
		TRACE("Failed to load picture\n");

		// Set Frame title back to default
		GetParent()->SendMessageW(UWM_FILELOADED, 0, (LPARAM)LoadString(IDW_MAIN).c_str());
		return FALSE;
	}
}

void CView::OnDraw(CDC* pDC)
{
	Paint(*pDC);
}


LRESULT CView::OnDropFiles(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HDROP hDrop = (HDROP)wParam;
	UINT nLength = DragQueryFile(hDrop, 0, 0, 0);

	if (nLength > 0)
	{
		CString FileName;
		DragQueryFile(hDrop, 0, FileName.GetBuffer(nLength), nLength+1);
		FileName.ReleaseBuffer();

		if ( !LoadPictureFile(FileName) )
			NewPictureFile();

		DragFinish(hDrop);
	}
	return 0L;
}

void CView::OnInitialUpdate()
{
	// Set the window background to black
	m_Brush.CreateSolidBrush(RGB(0,0,0));
	SetClassLongPtr(GCLP_HBRBACKGROUND, (LONG_PTR)m_Brush.GetHandle());

	// Support Drag and Drop on this window
	DragAcceptFiles(TRUE);
}

LRESULT CView::OnHScroll(WPARAM wParam, LPARAM /*lParam*/)
{
	int xNewPos;

	switch (LOWORD(wParam))
	{
		case SB_PAGEUP: // User clicked the scroll bar shaft left of the scroll box.
			xNewPos = m_xCurrentScroll - 50;
			break;

		case SB_PAGEDOWN: // User clicked the scroll bar shaft right of the scroll box.
			xNewPos = m_xCurrentScroll + 50;
			break;

		case SB_LINEUP: // User clicked the left arrow.
			xNewPos = m_xCurrentScroll - 5;
			break;

		case SB_LINEDOWN: // User clicked the right arrow.
			xNewPos = m_xCurrentScroll + 5;
			break;

		case SB_THUMBPOSITION: // User dragged the scroll box.
			xNewPos = HIWORD(wParam);
			break;

		case SB_THUMBTRACK: // User dragging the scroll box.
			xNewPos = HIWORD(wParam);
			break;

		default:
			xNewPos = m_xCurrentScroll;
	}

	// Scroll the window.
	xNewPos = MAX(0, xNewPos);
	xNewPos = MIN( xNewPos, GetImageRect().Width() - GetClientRect().Width() );
	int xDelta = xNewPos - m_xCurrentScroll;
	m_xCurrentScroll = xNewPos;
	ScrollWindowEx(-xDelta, 0,  NULL, NULL, NULL, NULL, SW_INVALIDATE);

	// Reset the scroll bar.
	SCROLLINFO si = {0};
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.fMask  = SIF_POS;
	si.nPos   = m_xCurrentScroll;
	SetScrollInfo(SB_HORZ, si, TRUE);

	return 0L;
}

LRESULT CView::OnVScroll(WPARAM wParam, LPARAM /*lParam*/)
{
	int yNewPos;

	switch (LOWORD(wParam))
	{
		case SB_PAGEUP: // User clicked the scroll bar shaft above the scroll box.
			yNewPos = m_yCurrentScroll - 50;
			break;

		case SB_PAGEDOWN: // User clicked the scroll bar shaft below the scroll box.
			yNewPos = m_yCurrentScroll + 50;
			break;

		case SB_LINEUP: // User clicked the top arrow.
			yNewPos = m_yCurrentScroll - 5;
			break;

		case SB_LINEDOWN: // User clicked the bottom arrow.
			yNewPos = m_yCurrentScroll + 5;
			break;

		case SB_THUMBPOSITION: // User dragged the scroll box.
			yNewPos = HIWORD(wParam);
			break;

		case SB_THUMBTRACK: // User dragging the scroll box.
			yNewPos = HIWORD(wParam);
			break;

		default:
			yNewPos = m_yCurrentScroll;
	}

	// Scroll the window.
	yNewPos = MAX(0, yNewPos);
	yNewPos = MIN( yNewPos, GetImageRect().Height() - GetClientRect().Height() );
	int yDelta = yNewPos - m_yCurrentScroll;
	m_yCurrentScroll = yNewPos;
	ScrollWindowEx(0, -yDelta, NULL, NULL, NULL, NULL, SW_INVALIDATE);

	// Reset the scroll bar.
	SCROLLINFO si = {0};
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.cbSize = sizeof(si);
	si.fMask  = SIF_POS;
	si.nPos   = m_yCurrentScroll;
	SetScrollInfo(SB_VERT, si, TRUE);

	return 0;
}

LRESULT CView::OnWindowPosChanged(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_pPicture)
	{
		CRect rcImage = GetImageRect();
		DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
		DWORD dwExStyle = (DWORD)GetWindowLongPtr(GWL_EXSTYLE);
		AdjustWindowRectEx(&rcImage, dwStyle, FALSE, dwExStyle);

		CRect rcView = GetClientRect();
		AdjustWindowRectEx(&rcView, dwStyle, FALSE, dwExStyle);

		SCROLLINFO si = {0};
		si.cbSize = sizeof(si);
		si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin   = 0;

		if (rcView.Width()  >= rcImage.Width())
		{
			m_xCurrentScroll = 0;
			ShowScrollBar(SB_HORZ, FALSE);
		}
		else
		{
			si.nMax   = rcImage.Width();
			si.nPage  = rcView.Width();
			si.nPos   = m_xCurrentScroll;
			SetScrollInfo(SB_HORZ, si, TRUE);
			ShowScrollBar(SB_HORZ, TRUE);
		}

		if (rcView.Height() >= rcImage.Height())
		{
			m_yCurrentScroll = 0;
			ShowScrollBar(SB_VERT, FALSE);
		}
		else
		{
			si.nMax   = rcImage.Height();
			si.nPage  = rcView.Height();
			si.nPos   = m_yCurrentScroll;
			SetScrollInfo(SB_VERT, si, TRUE);
			ShowScrollBar(SB_VERT, TRUE);
		}

		int xNewPos = MIN(m_xCurrentScroll, rcImage.Width() - rcView.Width());
		m_xCurrentScroll = MAX(xNewPos, 0);
		int yNewPos = MIN(m_yCurrentScroll, rcImage.Height() - rcView.Height());
		m_yCurrentScroll = MAX(yNewPos, 0);

		// Paint the window directly to eliminate flicker
		CClientDC dcView(this);
		Paint(dcView);
	}

	return 0L;
}

void CView::Paint(HDC hDC)
{
	if (m_pPicture)
	{
		// get width and height of picture
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		// convert himetric to pixels
		int nWidth	= MulDiv(hmWidth, GetDeviceCaps(hDC, LOGPIXELSX), HIMETRIC_INCH);
		int nHeight	= MulDiv(hmHeight, GetDeviceCaps(hDC, LOGPIXELSY), HIMETRIC_INCH);

		// calculate himetric start pos
		int xStart = MulDiv(m_xCurrentScroll, HIMETRIC_INCH, GetDeviceCaps(hDC, LOGPIXELSX));
		int yStart = MulDiv(m_yCurrentScroll, HIMETRIC_INCH, GetDeviceCaps(hDC, LOGPIXELSY));

		// Render the picture to the DC
		m_pPicture->Render(hDC, 0, 0, nWidth, nHeight, xStart, hmHeight - yStart, hmWidth, -hmHeight, NULL);
	}
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Set the Window Class name
	cs.lpszClass = _T("PictureView");

	cs.style = WS_CHILD | WS_HSCROLL | WS_VSCROLL ;

	// Set the extended style
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::SavePicture(LPCTSTR szFile)
{
	// get a IPictureDisp interface from your IPicture pointer
	IPictureDisp *pDisp = NULL;

	if (SUCCEEDED(m_pPicture->QueryInterface(IID_IPictureDisp,  (void**) &pDisp)))
	{
		// Save the IPicture image as a bitmap
		OleSavePictureFile(pDisp,  T2BSTR(szFile));
		pDisp->Release();
	}
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DROPFILES:			return OnDropFiles(wParam, lParam);
	case WM_HSCROLL:			return OnHScroll(wParam, lParam);
	case WM_VSCROLL:			return OnVScroll(wParam, lParam);
	case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(wParam, lParam);
	}

	// Pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}


