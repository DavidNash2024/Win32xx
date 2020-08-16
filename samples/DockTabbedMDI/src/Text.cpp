///////////////////////////////////////////////////
// Text.cpp -  Definitions for the CViewText,
//              and CDockText classes
//

#include "stdafx.h"
#include "Text.h"
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
    m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));

    SetFont(m_font, FALSE);
    SetWindowText(_T("Text Edit Window\r\n\r\n You can type some text here ..."));
}

// Respond to keyboard accelerator keys.
BOOL CViewText::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDM_EDIT_COPY:     OnEditCopy();   return TRUE;
    case IDM_EDIT_PASTE:    OnEditPaste();  return TRUE;
    case IDM_EDIT_CUT:      OnEditCut();    return TRUE;
    case IDM_EDIT_DELETE:   OnEditDelete(); return TRUE;
    case IDM_EDIT_REDO:     OnEditRedo();   return TRUE;
    case IDM_EDIT_UNDO:     OnEditUndo();   return TRUE;
    }

    // return FALSE for unhandled commands
    return FALSE;
}

// Copies the selected text to the windows clipboard.
void CViewText::OnEditCopy()
{
    Copy();
}

// Copies text from the windows clipboard.
void CViewText::OnEditPaste()
{
    PasteSpecial(CF_TEXT);
}

// Deletes the selected text and copies it to the windows clipboard.
void CViewText::OnEditCut()
{
    Cut();
}

// Deletes the selected text.
void CViewText::OnEditDelete()
{
    Clear();
}

// Redoes the next action in the control's redo queue.
void CViewText::OnEditRedo()
{
    Redo();
}

// Undoes the last edit control operation in the control's undo queue.
void CViewText::OnEditUndo()
{
    Undo();
}

// Sets the CREATESTRUCT parameters before the window is created.
void CViewText::PreCreate(CREATESTRUCT& cs)
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
                WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
}

////////////////////////////////////
// CContainText function definitions

// Constructor.
CContainText::CContainText()
{
    SetDockCaption (_T("Text View - Docking container"));
    SetTabText(_T("Text"));
    SetTabIcon(IDI_TEXT);
    SetView(m_viewText);
}

/////////////////////////////////
// CDockText function definitions
//

// Constructor.
CDockText::CDockText()
{
    // Set the view window to our edit control
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(8);
}

