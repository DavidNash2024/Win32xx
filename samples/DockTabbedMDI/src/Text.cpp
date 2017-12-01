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
    m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));

    SendMessage(WM_SETFONT, reinterpret_cast<WPARAM>(m_Font.GetHandle()), 0);

    SetWindowText(_T("Text Edit Window\r\n\r\n You can type some text here ..."));
}

BOOL CViewText::OnCommand(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    
    UINT nID = LOWORD(wParam);
    switch (nID)
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
    SendMessage(WM_COPY, 0, 0);
}

void CViewText::OnEditPaste()
{
    SendMessage(EM_PASTESPECIAL, CF_TEXT, 0);
}

void CViewText::OnEditCut()
{
    SendMessage(WM_CUT, 0, 0);
}

void CViewText::OnEditDelete()
{
    SendMessage(WM_CLEAR, 0, 0);
}

void CViewText::OnEditRedo()
{
    SendMessage(EM_REDO, 0, 0);
}

void CViewText::OnEditUndo()
{
    SendMessage(EM_UNDO, 0, 0);
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
    SetView(m_ViewText);
} 


//////////////////////////////////////////////
//  Definitions for the CDockText class
CDockText::CDockText()
{
    // Set the view window to our edit control
    SetView(m_View);

    // Set the width of the splitter bar
    SetBarWidth(8);
}

