//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"
#include "FastGDIApp.h"
#include "resource.h"


CView::CView()
{
}

CView::~CView()
{
}

BOOL CView::LoadFileImage(LPCTSTR szFilename)
{
	// Only bitmap images (bmp files) can be loaded
	
	m_bmImage.DeleteObject();
	CSize szTotal;

	if (szFilename)
	{
		m_bmImage.LoadImage(szFilename, LR_LOADFROMFILE);
		if (!m_bmImage.GetHandle())
		{
			CString str("Failed to load file:  ");
			str += szFilename;
			MessageBox(str, _T("File Load Error"), MB_ICONWARNING);
		}
	}

	if (m_bmImage.GetHandle())
	{
		// Set the image scroll size
		szTotal.cx = GetImageRect().Width();
		szTotal.cy = GetImageRect().Height();
	}
	else
	{
		// Disable scrolling
		szTotal = CSize(0, 0);
		Invalidate();
	}

	SetScrollSizes(szTotal);
	return (m_bmImage.GetHandle()!= 0);
}

BOOL CView::SaveFileImage(LPCTSTR pszFile)
 {
	 CFile File;
	 BOOL bResult = FALSE;
	 try
	 {
		 File.Open(pszFile, OPEN_ALWAYS);
	 
		// Create our LPBITMAPINFO object
		CBitmapInfoPtr pbmi(m_bmImage);

		// Create the reference DC for GetDIBits to use
		CMemDC MemDC(NULL);

		// Use GetDIBits to create a DIB from our DDB, and extract the colour data
		MemDC.GetDIBits(m_bmImage, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
		std::vector<byte> vBits(pbmi->bmiHeader.biSizeImage, 0);
		byte* lpvBits = &vBits.front();

		MemDC.GetDIBits(m_bmImage, 0, pbmi->bmiHeader.biHeight, lpvBits, pbmi, DIB_RGB_COLORS);

		LPBITMAPINFOHEADER pbmih = &pbmi->bmiHeader;
		BITMAPFILEHEADER hdr;
		ZeroMemory(&hdr, sizeof(BITMAPFILEHEADER));
		hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
		hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof(RGBQUAD) + pbmih->biSizeImage);
		hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof (RGBQUAD);

		File.Write((LPCVOID) &hdr, sizeof(BITMAPFILEHEADER));
		File.Write((LPCVOID) pbmih, sizeof(BITMAPINFOHEADER) + pbmih->biClrUsed * sizeof (RGBQUAD));
		File.Write((LPCVOID) lpvBits, (int) pbmih->biSizeImage);

		if (File.GetLength() == sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbmih->biClrUsed * sizeof (RGBQUAD) + (int) pbmih->biSizeImage)
			bResult = TRUE;
	 }

	 catch (const CFileException& e)
	 {
		 CString str = CString("Failed to save file: ") + e.GetFilePath();
		 MessageBox(str, AtoT(e.what()), MB_OK);
	 }

	return bResult;
}

CRect CView::GetImageRect()
{
	BITMAP bm;
	m_bmImage.GetObject(sizeof(BITMAP), &bm);

	CRect rc;
	rc.right = bm.bmWidth;
	rc.bottom = bm.bmHeight;

	return rc;
}

void CView::OnDraw(CDC& dc)
{
	if (m_bmImage.GetHandle())
	{
		dc.SelectObject(m_bmImage);
	}
	else
	{
		// There is no image, so display a hint to get one
		CRect rc = GetClientRect();
		dc.DrawText(_T("Use the Menu or ToolBar to open a Bitmap File"), -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

LRESULT CView::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(lParam);

	HDROP hDrop = (HDROP)wParam;
	UINT nLength = DragQueryFile(hDrop, 0, 0, 0);

	if (nLength > 0)
	{
		CString FileName;
		DragQueryFile(hDrop, 0, FileName.GetBuffer(nLength), nLength+1);
		FileName.ReleaseBuffer();
		DragFinish(hDrop);

		CMainFrame& Frame = GetFrameApp().GetMainFrame();

		if ( !Frame.LoadFile(FileName) )	
		{
			TRACE ("Failed to load "); TRACE(FileName); TRACE("\n");
			SetScrollSizes(CSize(0,0));
			Invalidate();
		}
	}
	
	return 0L;
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called after the window is created
	TRACE("View window created\n");

	// Support Drag and Drop on this window
	DragAcceptFiles(TRUE);
}

void CView::PreCreate(CREATESTRUCT& cs)
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
	case WM_DROPFILES:			return OnDropFiles(uMsg, wParam, lParam);
	}

	// Pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

