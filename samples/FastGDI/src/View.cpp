//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"
#include "resource.h"


CView::CView() : m_hbmImage(NULL), m_xCurrentScroll(0), m_yCurrentScroll(0)
{
}

CView::~CView()
{
	::DeleteObject(m_hbmImage);
}

BOOL CView::FileOpen(LPCTSTR szFilename)
{
	if (szFilename)
	{
		m_hbmImage = (HBITMAP)::LoadImage(GetApp()->GetInstanceHandle(), 
					szFilename, 0, 0, IMAGE_BITMAP, LR_LOADFROMFILE);
	}
	else
		m_hbmImage = NULL;
	
	return (BOOL)m_hbmImage;
}

BOOL CView::FileSave(LPCTSTR pszFile) 
 { 
	 HANDLE hFile = CreateFile(pszFile, GENERIC_READ | GENERIC_WRITE, (DWORD) 0, NULL, 
                   OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL);

	 if (hFile)
	 {
		// Create our LPBITMAPINFO object
		CBitmapInfoPtr pbmi(m_hbmImage);
				
		// Create the reference DC for GetDIBits to use
		CDC MemDC = CreateCompatibleDC(NULL);

		// Use GetDIBits to create a DIB from our DDB, and extract the colour data
		MemDC.GetDIBits(m_hbmImage, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
		std::vector<byte> vBits(pbmi->bmiHeader.biSizeImage, 0);
		byte* lpvBits = &vBits.front();

		MemDC.GetDIBits(m_hbmImage, 0, pbmi->bmiHeader.biHeight, lpvBits, pbmi, DIB_RGB_COLORS);		 

		LPBITMAPINFOHEADER pbmih = &pbmi->bmiHeader;
		BITMAPFILEHEADER hdr = {0};
		hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
		hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof(RGBQUAD) + pbmih->biSizeImage);
		hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof (RGBQUAD); 

		DWORD dwBytesWritten;
		BOOL bResult = WriteFile(hFile, (LPCVOID) &hdr, sizeof(BITMAPFILEHEADER),  (LPDWORD) &dwBytesWritten,  NULL);
		if (bResult) 
			bResult = WriteFile(hFile, (LPCVOID) pbmih, sizeof(BITMAPINFOHEADER) + pbmih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwBytesWritten, (NULL));
		if (bResult) 
			bResult = WriteFile(hFile, (LPCVOID) lpvBits, (int) pbmih->biSizeImage, (LPDWORD) &dwBytesWritten, NULL);

		CloseHandle(hFile);
		return bResult;
	}

	return FALSE;
}

CRect CView::GetImageRect()
{
	BITMAP bm;
	::GetObject(m_hbmImage, sizeof(BITMAP), &bm);

	CRect rc;
	rc.right = bm.bmWidth;
	rc.bottom = bm.bmHeight;
	
	return rc;
}

void CView::OnDraw(CDC* pDC)
{
	if (m_hbmImage)
	{
		// We have an image, so display it
		CDC memDC = ::CreateCompatibleDC(*pDC);
		CRect rcView = GetClientRect();
		memDC.AttachBitmap(m_hbmImage);
		pDC->BitBlt(0, 0, rcView.Width(), rcView.Height(), &memDC, m_xCurrentScroll, m_yCurrentScroll, SRCCOPY);
		memDC.DetachBitmap(); 
	}
	else
	{
		// There is no image, so display a hint to get one
		CRect rc = GetClientRect();
		pDC->DrawText(_T("Use the Menu or ToolBar to open a Bitmap File"), -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called immediately after the window is created
	TRACE(_T("View window created\n"));

	ShowScrollBar(SB_BOTH, FALSE);
}

void CView::OnHScroll(WPARAM wParam, LPARAM lParam)
{ 
	UNREFERENCED_PARAMETER(lParam);
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
	si.cbSize = sizeof(si); 
	si.fMask  = SIF_POS; 
	si.nPos   = m_xCurrentScroll; 
	SetScrollInfo(SB_HORZ, si, TRUE); 
} 

void CView::OnVScroll(WPARAM wParam, LPARAM lParam)
{  
	UNREFERENCED_PARAMETER(lParam);
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
} 

void CView::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (m_hbmImage) 
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
		xNewPos = MAX(xNewPos, 0);
		int xDelta = xNewPos - m_xCurrentScroll;

		int yNewPos = MIN(m_yCurrentScroll, rcImage.Height() - rcView.Height());
		yNewPos = MAX(yNewPos, 0);
		int yDelta = yNewPos - m_yCurrentScroll;

		ScrollWindowEx(-xDelta, -yDelta, NULL, NULL, NULL, NULL, SW_INVALIDATE);
		m_xCurrentScroll = xNewPos;
		m_yCurrentScroll = yNewPos;
	}
	else		
		Invalidate();	// Keep the text centered in the window
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Set the Window Class name
	cs.lpszClass = _T("View");

	cs.style = WS_CHILD | WS_HSCROLL | WS_VSCROLL ;

	// Set the extended style
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_WINDOWPOSCHANGED:
		OnWindowPosChanged(wParam, lParam);	
		break;
		    
	case WM_HSCROLL: 
		OnHScroll(wParam, lParam);
		break; 
	 
	case WM_VSCROLL: 
		OnVScroll(wParam, lParam);
        break;
	}

	// Pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

