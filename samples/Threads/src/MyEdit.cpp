/////////////////////////////
// MyEdit.cpp
//

#include "stdafx.h"
#include "MyEdit.h"

////////////////////////////////
// CMyEdit function definitions.
//

// Constructor.
CMyEdit::CMyEdit()
{
    LoadCommonControls();

    // Create the font for the Edit window
    m_font.CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
}

// Called after the edit window is created.
void CMyEdit::OnAttach()
{
    // Set the font for the Edit window
    SetFont(m_font, FALSE);
}

// Called before the edit window is created.
void CMyEdit::PreCreate(CREATESTRUCT& cs)
{
    // Specify the edit window's style before the window is created
    cs.style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL;
}
