//////////////////////////////////////////////
// SimpleMDIChild.cpp
//  Definitions for the CSimpleView and CSimpleMDIChild classes

#include "stdafx.h"
#include "SimpleMDIChild.h"
#include "resource.h"

///////////////////////////////////
// CSimpleView function definitions
//

// Constructor.
CSimpleView::CSimpleView() : m_color(RGB(0,0,255))
{
}

// Called when part of the window needs to be redrawn.
void CSimpleView::OnDraw(CDC& dc)
{
    // Use the message font for Windows 7 and higher.
    if (GetWinVersion() >= 2601)
    {
        NONCLIENTMETRICS info = GetNonClientMetrics();
        LOGFONT lf = info.lfMessageFont;
        dc.CreateFontIndirect(lf);
    }

    //Centre some text in our view window
    CRect rc = GetClientRect();
    dc.SetTextColor(m_color);
    dc.DrawText(L"View Window", -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

// Respond to a mouse click on the window.
LRESULT CSimpleView::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the window is resized.
LRESULT CSimpleView::OnSize(UINT, WPARAM, LPARAM)
{
    // Force the window to be repainted
    Invalidate();
    return 0;
}

// Process the simple view's window messages.
LRESULT CSimpleView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
        case WM_SIZE:               return OnSize(msg, wparam, lparam);
        }

        // Do default processing for other messages.
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


///////////////////////////////////////
// CSimpleMDIChild function definitions
//

// Constructor.
CSimpleMDIChild::CSimpleMDIChild()
{
    // Set m_view as the view window of the MDI child
    SetView(m_view);

    // Set the menu for this MDI child
    m_menu.LoadMenu(L"MdiMenuView");
    SetHandles(m_menu, nullptr);
}

// Called when the window is created.
int CSimpleMDIChild::OnCreate(CREATESTRUCT& cs)
{
    // Set the window caption
    SetWindowText(L"Simple Window");

    // Set the window icons
    SetIconLarge(IDI_VIEW);
    SetIconSmall(IDI_VIEW);

    return CMDIChild::OnCreate(cs);
}

// Respond to menu and toolbar input forwarded from the MDI Frame.
BOOL CSimpleMDIChild::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDM_COLOR_BLACK:
        m_view.SetColor(RGB(0,0,0));
        Invalidate();
        return TRUE;
    case IDM_COLOR_RED:
        m_view.SetColor(RGB(255, 0, 0));
        Invalidate();
        return TRUE;
    case IDM_COLOR_GREEN:
        m_view.SetColor(RGB(0, 255, 0));
        Invalidate();
        return TRUE;
    case IDM_COLOR_BLUE:
        m_view.SetColor(RGB(0, 0, 255));
        Invalidate();
        return TRUE;
    case IDM_COLOR_WHITE:
        m_view.SetColor(RGB(255, 255, 255));
        Invalidate();
        return TRUE;
    }

    return FALSE;
}

// Process the MDI child's window messages.
LRESULT CSimpleMDIChild::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here.
    //  }

        // Do default processing for other messages.
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

