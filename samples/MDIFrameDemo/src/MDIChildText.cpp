//////////////////////////////////////////////
// MDIChildText.cpp
//  Definitions for the CMDIChildText class

#include "stdafx.h"
#include "MDIChildText.h"
#include "resource.h"


CViewText::CViewText()
{
	m_hRichEdit = ::LoadLibrary(_T("RICHED32.DLL"));
    if (m_hRichEdit == 0)
    {
		::MessageBox(NULL,_T("CRichView::CRichView  Failed to load RICHED32.DLL"), _T(""), MB_ICONWARNING);
    }
}

CViewText::~CViewText()
{
	// Cleanup
	if (m_hRichEdit) ::FreeLibrary(m_hRichEdit);
}

int CViewText::OnCreate(LPCREATESTRUCT pcs)
{
	UNREFERENCED_PARAMETER(pcs);

	//Set font
	if (!m_Font)
	{
		m_Font.CreateFont(16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
	}
	
	SendMessage(WM_SETFONT, (WPARAM)m_Font.GetHandle(), 0L);
	return 0;
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

BOOL CMDIChildText::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
	{
	case IDM_EDIT_COPY:
		GetView()->SendMessage(WM_COPY, 0, 0);
		return TRUE;
	case IDM_EDIT_PASTE:
		GetView()->SendMessage(WM_PASTE, 0, 0);
		return TRUE;
	}

	return FALSE;
}


int CMDIChildText::OnCreate(LPCREATESTRUCT pcs)
{
	UNREFERENCED_PARAMETER(pcs);
	m_TextView.Create(this);

	SetWindowText(_T("Text Window"));
	SetIconLarge(IDI_TEXT);
	SetIconSmall(IDI_TEXT);
	
	return 0;
}

LRESULT CMDIChildText::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	m_TextView.SetFocus();
	return 0L;
}

LRESULT CMDIChildText::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETFOCUS:	return OnSetFocus(wParam, lParam);
	}

	// Do default processing for other messages
	return WndProcDefault(uMsg, wParam, lParam);
}
