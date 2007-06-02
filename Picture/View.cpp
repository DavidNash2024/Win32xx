//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"

#define HIMETRIC_INCH	2540

CView::CView() : m_hBrush(NULL), m_pPicture(NULL)
{
	::CoInitialize(NULL);
}

CView::~CView()
{
	if (m_hBrush)
		::DeleteObject(m_hBrush);
	if (m_pPicture)
		m_pPicture->Release();

	::CoUninitialize();
}

void CView::LoadPictureFile(LPCTSTR szFile)
{
	// Create IPicture from image file
	if (m_pPicture)
		m_pPicture->Release();

	if (S_OK != ::OleLoadPicturePath(T2OLE(szFile), NULL, 0, 0,
						IID_IPicture, (LPVOID *)&m_pPicture))
	{
		TRACE(_T("Failed to load picture"));
	}
	
	::InvalidateRect(m_hWnd, NULL, TRUE);
}

void CView::OnInitialUpdate()
{
	// Set the window background to black
	m_hBrush = ::CreateSolidBrush(RGB(0,0,0));
	::SetClassLong(m_hWnd, GCL_HBRBACKGROUND, (LONG)m_hBrush);
}

void CView::OnPaint(HDC hDC)
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
		
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		
		// display picture using IPicture::Render
		m_pPicture->Render(hDC, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, &rc);
	}
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Set the Window Class name
	cs.lpszClass = _T("PictureView");

	// Set the extended style
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

LPOLESTR CView::T2OLE(LPCTSTR szString)
{

#ifdef UNICODE
	lstrcpyn((LPOLESTR)m_OleString, szString, MAX_STRING_SIZE);
#else
	mbstowcs((LPOLESTR)m_OleString, szString, MAX_STRING_SIZE);
#endif

	return (LPOLESTR)m_OleString;
}

LRESULT CView::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

//	switch (uMsg)
//	{

//	}

	// Use the frame default message handling for remaining messages
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}


