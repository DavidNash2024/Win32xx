/////////////////////////////////////////////
// RichView.cpp

#include "richview.h"
#include <richedit.h>


CRichView::CRichView(void) : m_hFont(NULL), m_hInstRichEdit(NULL)
{    
	m_hInstRichEdit = ::LoadLibrary(TEXT("RICHED32.DLL"));
    if (!m_hInstRichEdit)
    {
		::MessageBox(NULL,TEXT("CRichView::CRichView  Failed to load RICHED32.DLL"), TEXT(""), MB_ICONWARNING);
    }
}

CRichView::~CRichView(void)
{
	if (m_hInstRichEdit)
		::FreeLibrary(m_hInstRichEdit);
	if(m_hFont)
		::DeleteObject(m_hFont);
}

void CRichView::PreCreate(CREATESTRUCT &cs)
{
	cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD | 
				WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;

	cs.dwExStyle = WS_EX_CLIENTEDGE;
	cs.lpszClass = TEXT("RichEdit");
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
		m_hFont = ::CreateFont(16,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,TEXT("Courier New"));
	::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)(HFONT)m_hFont,0);
}

