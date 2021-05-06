/////////////////////////////
// SimpleMDIChild.cpp
//

#include "stdafx.h"
#include "SimpleMDIChild.h"
#include "Resource.h"

////////////////////////////////////
// CSimpleView function definitions.
//
CSimpleView::CSimpleView() : m_color(RGB(0,0,255))
{
}

void CSimpleView::OnDraw(CDC& dc)
{
    //Centre some text in our view window
    CRect rc = GetClientRect();
    dc.SetTextColor(m_color);
    dc.DrawText(L"View Window", -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CSimpleView::OnSize(UINT, WPARAM, LPARAM)
{
    // Force the window to be repainted
    Invalidate();
    return 0;
}

LRESULT CSimpleView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:   return OnSize(msg, wparam, lparam);
        }

        // Do default processing for other messages.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

// CSimpleMDIChild definitions
CSimpleMDIChild::CSimpleMDIChild()
{
    // Set m_View as the view window of the MDI child
    SetView(m_view);

    // Set the menu for this MDI child
    m_menu.LoadMenu(L"MdiMenuView");
    SetHandles(m_menu, 0);
}

CSimpleMDIChild::~CSimpleMDIChild()
{
}

int CSimpleMDIChild::OnCreate(CREATESTRUCT& cs)
{
    // Set the window caption
    SetWindowText( L"Simple Window" );

    // Set the window icons
    SetIconLarge(IDI_VIEW);
    SetIconSmall(IDI_VIEW);

    GetAncestor().SendMessage(UWM_SIMPLECREATED, (WPARAM)&m_view, 0);
    return CMDIChild::OnCreate(cs);
}

BOOL CSimpleMDIChild::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // Respond to menu and toolbar input

    UNREFERENCED_PARAMETER(lparam);

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

LRESULT CSimpleMDIChild::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {

    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here
    //  }

        // Do default processing for other messages/
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}
