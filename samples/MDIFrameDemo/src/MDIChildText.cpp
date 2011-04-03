//////////////////////////////////////////////
// MDIChildText.cpp
//  Definitions for the CMDIChildText class

#include "stdafx.h"
#include "MDIChildText.h"
#include "resource.h"


CViewText::CViewText()
{
	m_hRichEdit = ::LoadLibrary(_T("RICHED32.DLL"));
    if (!m_hRichEdit)
    {
		::MessageBox(NULL,_T("CRichView::CRichView  Failed to load RICHED32.DLL"), _T(""), MB_ICONWARNING);
    }
}

CViewText::~CViewText()
{
	// Cleanup
	if (m_hRichEdit) ::FreeLibrary(m_hRichEdit);
}

void CViewText::OnCreate()
{
	//Set font
	if (!m_Font)
	{
		m_Font.CreateFont(16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
	}
	
	SendMessage(WM_SETFONT, (WPARAM)m_Font.GetFont(), 0L);
}

void CViewText::PreCreate(CREATESTRUCT &cs)
{
	cs.lpszClass = _T("RichEdit");
	cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD | 
				WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
}


CMDIChildText::CMDIChildText()
{
	HINSTANCE hResource = GetApp()->GetResourceHandle();
	HMENU hChildMenu = LoadMenu(hResource, _T("MdiMenuText"));
	SetHandles(hChildMenu, NULL);
	SetView(m_TextView);
}

CMDIChildText::~CMDIChildText()
{	
}

void CMDIChildText::OnCreate()
{
	m_TextView.Create(this);
}

void CMDIChildText::OnInitialUpdate()
{
	SetWindowText(_T("Text Window"));
	SetIconLarge(IDI_TEXT);
	SetIconSmall(IDI_TEXT);
}

LRESULT CMDIChildText::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETFOCUS:
		m_TextView.SetFocus();
		break;
	}

	// Do default processing for other messages
	return WndProcDefault(uMsg, wParam, lParam);
}
