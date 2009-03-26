/////////////////////////////////////////////
// RichView.cpp

#include "richview.h"
#include <richedit.h>


CRichView::CRichView(void) : m_hFont(NULL), m_hInstRichEdit(NULL)
{ 
	// Changed from RichEdit v1.0 to RichEdit v2.0 for Unicode support!

	m_hInstRichEdit = ::LoadLibrary(_T("Riched20.dll")); // RichEdit ver 2.0
    if (!m_hInstRichEdit)
    {
		::MessageBox(NULL,_T("CRichView::CRichView  Failed to load Riched20.dll"), _T(""), MB_ICONWARNING);
    }
}

CRichView::~CRichView(void)
{
	// Destroy the window first
	Destroy();

	// Now do remaining clean up
	if (m_hInstRichEdit)
		::FreeLibrary(m_hInstRichEdit);
	if(m_hFont)
		::DeleteObject(m_hFont);
}

void CRichView::PreCreate(CREATESTRUCT &cs)
{
	cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD | 
				WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;

	cs.dwExStyle = WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES;
	cs.lpszClass = RICHEDIT_CLASS; // RichEdit ver 2.0
}

void CRichView::OnInitialUpdate(void)
{
	//Before EM_LIMITTEXT is called, the default limit for the amount
	//of text a user can enter in an edit control is 32,767 characters
	//increase the text limit of the rich edit window
	::SendMessage(m_hWnd, EM_LIMITTEXT, (WPARAM)1.0e9, 0);

	//Determine which messages will be passed to the parent
	UINT uMask = ENM_KEYEVENTS | ENM_DROPFILES ;
	::SendMessage(m_hWnd, EM_SETEVENTMASK, 0, uMask);

	SetFontDefaults();
}

void CRichView::SetFontDefaults()
{
	//Set font
	if (!m_hFont)
		m_hFont = ::CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));

	::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)(HFONT)m_hFont,0);

	// Prevent Unicode characters from changing the font
	LRESULT lres = SendMessage(EM_GETLANGOPTIONS, 0, 0);
	lres &= ~IMF_AUTOFONT;
	SendMessage(EM_SETLANGOPTIONS, 0, lres);
}

