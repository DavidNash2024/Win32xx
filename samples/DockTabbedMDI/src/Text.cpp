///////////////////////////////////////////////////
// Text.cpp -  Definitions for the CViewText, 
//              and CDockText classes

#include "stdafx.h"
#include "Text.h"
#include "resource.h"


///////////////////////////////////////////////
// CViewText functions
CViewText::CViewText()
{
}

CViewText::~CViewText()
{
}

void CViewText::OnAttach()
{
    m_font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));

    SetFont(m_font, FALSE);
    SetWindowText(_T("Text Edit Window\r\n\r\n You can type some text here ..."));
}

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

void CViewText::OnEditCopy()
{
    Copy();
}

void CViewText::OnEditPaste()
{
    PasteSpecial(CF_TEXT);
}

void CViewText::OnEditCut()
{
    Cut();
}

void CViewText::OnEditDelete()
{
    Clear();
}

void CViewText::OnEditRedo()
{
    Redo();
}

void CViewText::OnEditUndo()
{
    Undo();
}

void CViewText::PreCreate(CREATESTRUCT& cs)
{
    cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD | 
                WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;
}


///////////////////////////////////////////////
// CContainText functions
CContainText::CContainText()
{    
    SetDockCaption (_T("Text View - Docking container"));
    SetTabText(_T("Text"));
    SetTabIcon(IDI_TEXT);
    SetView(m_viewText);
} 


//////////////////////////////////////////////
//  Definitions for the CDockText class
CDockText::CDockText()
{
    // Set the view window to our edit control
    SetView(m_view);

    // Set the width of the splitter bar
    SetBarWidth(8);
}

