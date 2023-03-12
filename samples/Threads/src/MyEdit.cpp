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
}

void CMyEdit::DPISetFont()
{
    m_font.CreatePointFont(100, _T("Courier New"));
    m_font = DPIScaleFont(m_font, 10);
    SetFont(m_font);
}


// Called after the edit window is created.
void CMyEdit::OnAttach()
{
    // Set the font for the Edit window
    DPISetFont();
}

// Called before the edit window is created.
void CMyEdit::PreCreate(CREATESTRUCT& cs)
{
    // Specify the edit window's style before the window is created
    cs.style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL;
}
