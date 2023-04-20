///////////////////////////////////////////////////
// Text.cpp -  Definitions for the CViewText,
//              and CDockText classes
//

#include "stdafx.h"
#include "Text.h"
#include "resource.h"

#ifndef WM_DPICHANGED_BEFOREPARENT
  #define WM_DPICHANGED_BEFOREPARENT      0x02E2
#endif

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
    CFont font;
    font.CreatePointFont(100, _T("Courier New"));
    SetFont(font);
    SetWindowText(_T("Text Edit Window\r\n\r\n You can type some text here ..."));

    // Advises the control to be per-monitor DPI aware.
    // This affects the initial font size on a second monitor with different DPI.
    SendMessage(WM_DPICHANGED_BEFOREPARENT);
}

// Respond to keyboard accelerator keys.
BOOL CViewText::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDM_EDIT_COPY:     return OnEditCopy();
    case IDM_EDIT_PASTE:    return OnEditPaste();
    case IDM_EDIT_CUT:      return OnEditCut();
    case IDM_EDIT_DELETE:   return OnEditDelete();
    case IDM_EDIT_REDO:     return OnEditRedo();
    case IDM_EDIT_UNDO:     return OnEditUndo();
    }

    // return FALSE for unhandled commands
    return FALSE;
}

// Copies the selected text to the windows clipboard.
BOOL CViewText::OnEditCopy()
{
    Copy();
    return TRUE;
}

// Copies text from the windows clipboard.
BOOL CViewText::OnEditPaste()
{
    PasteSpecial(CF_TEXT);
    return TRUE;
}

// Deletes the selected text and copies it to the windows clipboard.
BOOL CViewText::OnEditCut()
{
    Cut();
    return TRUE;
}

// Deletes the selected text.
BOOL CViewText::OnEditDelete()
{
    Clear();
    return TRUE;
}

// Redoes the next action in the control's redo queue.
BOOL CViewText::OnEditRedo()
{
    Redo();
    return TRUE;
}

// Undoes the last edit control operation in the control's undo queue.
BOOL CViewText::OnEditUndo()
{
    Undo();
    return TRUE;
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
    SetBarWidth(DPIScaleInt(8));
}

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CDockText::OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set the width of the splitter bar.
    SetBarWidth(DPIScaleInt(8));
    return CDocker::OnUserDPIChanged(msg, wparam, lparam);
}
