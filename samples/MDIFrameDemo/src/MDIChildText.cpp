//////////////////////////////////////////////
// MDIChildText.cpp
//  Definitions for the CMDIChildText class

#include "stdafx.h"
#include "MDIChildText.h"
#include "resource.h"

/////////////////////////////////
// CViewText function definitions
//

// Constructor.
CViewText::CViewText()
{
}

// Destructor.
CViewText::~CViewText()
{
}

// Called when a window handle (HWND) is attached to CViewText.
void CViewText::OnAttach()
{
    // Set the font.
    if (!m_font)
    {
        m_font.CreatePointFont(96, _T("Courier New"));
    }

    SetFont(m_font);
}

// Sets the CREATESTRUCT parameters before the window is created.
void CViewText::PreCreate(CREATESTRUCT& cs)
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
        WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
}

/////////////////////////////////////
// CMDIChildText function definitions
//

// Constructor.
CMDIChildText::CMDIChildText()
{
    m_menu.LoadMenu(_T("MdiMenuText"));
    SetHandles(m_menu, 0);
    SetView(m_textView);
}

// Destructor.
CMDIChildText::~CMDIChildText()
{
}

// Process menu and toolbar input forwarded from the MDI frame.
BOOL CMDIChildText::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_EDIT_COPY:
        GetView().SendMessage(WM_COPY, 0, 0);
        return TRUE;
    case IDM_EDIT_PASTE:
        GetView().SendMessage(WM_PASTE, 0, 0);
        return TRUE;
    }

    return FALSE;
}

// Called when the window is created.
int CMDIChildText::OnCreate(CREATESTRUCT& cs)
{
    SetWindowText(_T("Text Window"));
    SetIconLarge(IDI_TEXT);
    SetIconSmall(IDI_TEXT);

    return CMDIChild::OnCreate(cs);
}

