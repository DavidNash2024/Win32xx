/////////////////////////////
// MyEdit.cpp
//

#include "stdafx.h"
#include "MyEdit.h"

////////////////////////////////
// CMyEdit function definitions.
//
CMyEdit::CMyEdit()
// Constructor
{
    // Create the font for the Edit window
    m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
}

void CMyEdit::OnAttach()
// Called after the edit window is created
{
    // Permit the Edit window to display up to 128000 characters
    SendMessage(EM_SETLIMITTEXT, 128000, 0);

    // Set the font for the Edit window
    SetFont(m_font, FALSE);
}

void CMyEdit::PreCreate(CREATESTRUCT& cs)
// Called before the edit window is created
{
    // Specify the edit window's style before the window is created
    cs.style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL;
}
