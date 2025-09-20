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

// Called when a window handle (HWND) is attached to CViewText.
void CViewText::OnAttach()
{
    CFont font;
    font.CreatePointFont(100, L"Courier New");
    SetFont(font);
    SetWindowText(L"Text Edit Window\r\n\r\n You can type some text here ...");

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

// Handle the window's messages.
LRESULT CViewText::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}


////////////////////////////////////
// CContainText function definitions

// Constructor.
CContainText::CContainText()
{
    SetDockCaption (L"Text View - Docking container");
    SetTabText(L"Text");
    SetTabIcon(IDI_TEXT);
    SetView(m_viewText);
}

// Handle the window's messages.
LRESULT CContainText::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
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

// This function overrides CDocker::RecalcDockLayout to elimate jitter
// when the dockers are resized. The technique used here is is most
// appropriate for a complex arrangement of dockers.  It might not suite
// other docking applications. To support this technique the
// WS_EX_COMPOSITED extended style has been added to some view windows.
void CDockText::RecalcDockLayout()
{
    if (GetWinVersion() >= 3000)  // Windows 10 or later.
    {
        if (GetDockAncestor()->IsWindow())
        {
            GetTopmostDocker()->LockWindowUpdate();
            CRect rc = GetTopmostDocker()->GetViewRect();
            GetTopmostDocker()->RecalcDockChildLayout(rc);
            GetTopmostDocker()->UnlockWindowUpdate();
            GetTopmostDocker()->UpdateWindow();
        }
    }
    else
        CDocker::RecalcDockLayout();
}

// Handle the window's messages.
LRESULT CDockText::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}
