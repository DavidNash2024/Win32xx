//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"
#include "../../Win32++/gdi.h"
#include "resource.h"


CView::CView() : m_hbmImage(NULL)
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
		HBITMAP hbmSource = GetImage();

		// Create our LPBITMAPINFO object
		CBitmapInfoPtr pbmi(hbmSource);
				
		// Create the reference DC for GetDIBits to use
		CDC MemDC = CreateCompatibleDC(NULL);

		// Use GetDIBits to create a DIB from our DDB, and extract the colour data
		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
		byte* lpvBits = new byte[pbmi->bmiHeader.biSizeImage];
		if (NULL == lpvBits) throw std::bad_alloc();
		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, lpvBits, pbmi, DIB_RGB_COLORS);		 

		LPBITMAPINFOHEADER pbmih = &pbmi->bmiHeader;
		BITMAPFILEHEADER hdr = {0};
		hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
		hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof(RGBQUAD) + pbmih->biSizeImage);
		hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof (RGBQUAD); 

		DWORD dwTmp;
		DWORD cb = pbmi->bmiHeader.biSizeImage;                   // incremental count of bytes 
		BOOL bRes = WriteFile(hFile, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),  (LPDWORD) &dwTmp,  NULL);
		bRes = WriteFile(hFile, (LPVOID) pbmih, sizeof(BITMAPINFOHEADER) + pbmih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, ( NULL));
		bRes = WriteFile(hFile, (LPSTR) lpvBits, (int) cb, (LPDWORD) &dwTmp,NULL);

		CloseHandle(hFile);
		delete[] lpvBits;

		return TRUE;
	}

	return FALSE;
}

RECT CView::GetImageSize()
{
	BITMAP bm;
	::GetObject(m_hbmImage, sizeof(BITMAP), &bm);

	CRect rc;
	rc.right = bm.bmWidth;
	rc.bottom = bm.bmHeight;
	
	return rc;
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called immediately after the window is created
	TRACE(_T("View window created\n"));
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Set the Window Class name
	cs.lpszClass = _T("View");

	// Set the extended style
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::OnPaint(HDC hDC)
{
	if (m_hbmImage)
	{
		// We have an image, so display it
		CDC memDC = ::CreateCompatibleDC(hDC);
		CRect rcView = GetClientRect();
		memDC.AttachBitmap(m_hbmImage);
		::BitBlt(hDC, 0, 0, rcView.Width(), rcView.Height(), memDC, 0, 0, SRCCOPY);
		memDC.DetachBitmap(); 
	}
	else
	{
		// There is no image, so display a hint to get one
		CRect rc = GetClientRect();
		::DrawText(hDC, _T("Use the Menu or Toolbar to open a Bitmap File"), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_WINDOWPOSCHANGED:
		// Keep the text (if any) centered in the window
		if (NULL == m_hbmImage) Invalidate();
		break;
	}

	// Pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

