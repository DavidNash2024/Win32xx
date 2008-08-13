//////////////////////////////////////////////
// MDIChildText.cpp
//  Definitions for the CMDIChildText class

#include "MDIChildText.h"
#include "resource.h"

CRichEdit::CRichEdit() : m_hFont(NULL)
{
	m_hRichEdit = ::LoadLibrary(_T("RICHED32.DLL"));
    if (!m_hRichEdit)
    {
		::MessageBox(NULL,_T("CRichView::CRichView  Failed to load RICHED32.DLL"), _T(""), MB_ICONWARNING);
    }
}

CRichEdit::~CRichEdit()
{
	Destroy();
	if (m_hRichEdit) ::FreeLibrary(m_hRichEdit);
	if (m_hFont) ::DeleteObject(m_hFont);
}

void CRichEdit::OnCreate()
{
	//Set font
	if (!m_hFont)
	{
		m_hFont = ::CreateFont(16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
	}
	
	SendMessage(WM_SETFONT, (WPARAM)m_hFont,0);
}

void CRichEdit::PreCreate(CREATESTRUCT &cs)
{
	cs.lpszClass = _T("RichEdit");
	cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD | 
				WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
}


CMDIChildText::CMDIChildText()
{

	SetChildMenu(_T("MdiMenuText"));
}

CMDIChildText::~CMDIChildText()
{
	
}

void CMDIChildText::OnCreate()
{
	m_TextWindow.Create(m_hWnd);
}

void CMDIChildText::OnInitialUpdate()
{
	::SetWindowText(m_hWnd, _T("Text Window"));
	SetIconLarge(IDI_TEXT);
	SetIconSmall(IDI_TEXT);
}

LRESULT CMDIChildText::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETFOCUS:
		m_TextWindow.SetFocus();
		break;
	case WM_SIZE:
		{
			CRect rc = GetClientRect();
			m_TextWindow.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
		}
		break;		// Also do default processing
	}

	// Do default processing for other messages
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}
