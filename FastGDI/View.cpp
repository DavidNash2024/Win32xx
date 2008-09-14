//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"
#include "../Win32++/gdi.h"
#include "resource.h"


CView::CView() : m_hBitmap(NULL)
{

}

CView::~CView()
{
	::DeleteObject(m_hBitmap);
}

BOOL CView::FileOpen(LPCTSTR szFilename)
{
	Invalidate();
	if (szFilename)
	{
		m_hBitmap = (HBITMAP)::LoadImage(GetApp()->GetInstanceHandle(), 
					szFilename, 0, 0, IMAGE_BITMAP, LR_LOADFROMFILE);
	}
	else
		m_hBitmap = NULL;

	return (BOOL)m_hBitmap;
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
	if (m_hBitmap)
	{
		CDC memDC = ::CreateCompatibleDC(hDC);
		CRect rcView = GetClientRect();
		memDC.AttachBitmap(m_hBitmap);
		::BitBlt(hDC, 0, 0, rcView.Width(), rcView.Height(), memDC, 0, 0, SRCCOPY);
		memDC.DetachBitmap();
	}
	else
	{
		CRect rc = GetClientRect();

		// Centre some text in our view window
		::DrawText(hDC, _T("Load a Bitmap File from the menu or Toolbar"), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}


void CView::Tint()
{
	if (m_hBitmap)
	{
		CColourDialog Dialog(IDD_DIALOG1);
		Dialog.SetBitmap(m_hBitmap);

		Dialog.DoModal();
		TintBitmap(m_hBitmap, -80, -80, 0);

		// Copy the modified bitmap to the window
		CDC viewDC = GetDC(m_hWnd);
		CDC memDC = ::CreateCompatibleDC(viewDC);
		CRect rcView = GetClientRect();
		memDC.AttachBitmap(m_hBitmap);
		::BitBlt(viewDC, 0, 0, rcView.Width(), rcView.Height(), memDC, 0, 0, SRCCOPY);
		memDC.DetachBitmap();
	}
	else
		MessageBox(m_hWnd, _T("Open a Bitmap file first!"), _T("Errro"), MB_OK);
}

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_WINDOWPOSCHANGED:
		if (NULL == m_hBitmap) Invalidate();
		break;
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

