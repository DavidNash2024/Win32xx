/////////////////////////////////////////////
// RichView.cpp

#include "stdafx.h"
#include "richview.h"
#include <richedit.h>


CRichView::CRichView(void) : m_hInstRichEdit(NULL)
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
	// Destroy the window before freeing the DLL
	Destroy();

	// Now do remaining clean up
	if (m_hInstRichEdit)
		::FreeLibrary(m_hInstRichEdit);
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
	//increase the text limit of the rich edit window
	SendMessage(EM_LIMITTEXT, (WPARAM)-1, 0);
	SendMessage(EM_EXLIMITTEXT, 0, (LPARAM)-1);

	//Determine which messages will be passed to the parent
	UINT uMask = ENM_KEYEVENTS | ENM_DROPFILES ;
	SendMessage(EM_SETEVENTMASK, 0, uMask);

	SetFontDefaults();
}

void CRichView::SetFontDefaults()
{
	//Set font
	if (!m_Font.GetHandle())
		m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
	SetFont(&m_Font, FALSE);

// Required for Dev-C++
#ifndef IMF_AUTOFONT
  #define IMF_AUTOFONT			0x0002
#endif

	// Prevent Unicode characters from changing the font
	LRESULT lres = SendMessage(EM_GETLANGOPTIONS, 0, 0);
	lres &= ~IMF_AUTOFONT;
	SendMessage(EM_SETLANGOPTIONS, 0, lres);
}

