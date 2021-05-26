/////////////////////////////
// CMyEdit.cpp
//

#include "stdafx.h"
#include "MyEdit.h"

///////////////////////////////
// CMyEdit function definitions
//

// Constructor.
CMyEdit::CMyEdit()
{
    LoadCommonControls();

    // Create the font for the Edit window
    m_font.CreatePointFont(92, _T("Courier New"));
}

// Called when a window handle (HWND) is attached to CMyEdit.
void CMyEdit::OnAttach()
{
    // Set the font for the Edit window
    SetFont(m_font, FALSE);
}

// Set the CREATESTRUCT parameters before the window is created.
void CMyEdit::PreCreate(CREATESTRUCT& cs)
{
    // Specify the edit window's style before the window is created
    cs.style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL;
}
