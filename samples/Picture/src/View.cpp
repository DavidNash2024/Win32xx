//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class


#include "stdafx.h"
#include "view.h"
#include "PictureApp.h"
#include "resource.h"

#define HIMETRIC_INCH	2540

CView::CView() : m_pPicture(NULL)
{
	// Initializes the COM library on the current thread
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
	CClientDC dc(*this);
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
		
		// Turn scrolling off
		SetScrollSizes();
	}

	CMainFrame& Frame = GetPicApp().GetMainFrame();
	Frame.SetWindowText(LoadString(IDW_MAIN).c_str());
	Invalidate();
}

BOOL CView::LoadPictureFile(LPCTSTR szFile)
{
	if (m_pPicture)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
	}

	BOOL IsPictureLoaded;

	// Create IPicture from image file
	if (S_OK == ::OleLoadPicturePath(TtoOLE(szFile), NULL, 0, 0,	IID_IPicture, (LPVOID *)&m_pPicture))
	{
		CMainFrame& Frame = GetPicApp().GetMainFrame();
		Frame.SendMessage(UWM_FILELOADED, 0, (LPARAM)szFile);
		Invalidate();
		CSize size = CSize(GetImageRect().Width(), GetImageRect().Height());
		SetScrollSizes(size);
		IsPictureLoaded = TRUE;

		TRACE("Succesfully loaded: "); TRACE(szFile); TRACE("\n");
	}
	else
	{
		CString str("Failed to load: ");
		str += szFile;
		MessageBox(str, _T("Load Picture Failed"), MB_ICONWARNING);
		TRACE(str); TRACE("\n");

		// Set Frame title back to default
		CMainFrame& Frame = GetPicApp().GetMainFrame();
		Frame.SetWindowText(LoadString(IDW_MAIN).c_str());
		SetScrollSizes();
		IsPictureLoaded = FALSE;
	}

	return IsPictureLoaded;
}

int CView::OnCreate(CREATESTRUCT& cs)
{
	// Set the window background to black
	m_Brush.CreateSolidBrush(RGB(0,0,0));
	SetClassLongPtr(GCLP_HBRBACKGROUND, (LONG_PTR)m_Brush.GetHandle());

	// Set a black background brush for scrolling. 
	SetScrollBkgnd(m_Brush);

	// Support Drag and Drop on this window
	DragAcceptFiles(TRUE);

	return CWnd::OnCreate(cs);
}

void CView::OnDraw(CDC& dc)
{
	if (m_pPicture)
	{
		// get width and height of picture
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		// convert himetric to pixels
		int nWidth = MulDiv(hmWidth, GetDeviceCaps(dc, LOGPIXELSX), HIMETRIC_INCH);
		int nHeight = MulDiv(hmHeight, GetDeviceCaps(dc, LOGPIXELSY), HIMETRIC_INCH);

		// Render the picture to the DC
		m_pPicture->Render(dc, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, NULL);
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

		if ( !LoadPictureFile(FileName) )
			NewPictureFile();

		DragFinish(hDrop);
	}
	
	return 0L;
}

void CView::PreCreate(CREATESTRUCT& cs)
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
		OleSavePictureFile(pDisp,  TtoBSTR(szFile));
		pDisp->Release();
	}
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


