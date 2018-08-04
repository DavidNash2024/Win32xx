//////////////////////////////////////////////
// MDIChildSimple.cpp
//  Definitions for the CViewSimple and CMDIChildSimple classes

#include "stdafx.h"
#include "MDIChildSimple.h"
#include "resource.h"


// CViewSimple definitions
CViewSimple::CViewSimple() : m_color(RGB(0,0,255))
{
}

void CViewSimple::OnDraw(CDC& dc)
{
    //Centre some text in our view window
    CRect rc = GetClientRect();
    dc.SetTextColor(m_color);
    dc.DrawText(_T("View Window"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CViewSimple::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_SIZE:
        // Force the window to be repainted
        Invalidate();
        break;      // Also do default processing
    }

    // Do default processing for other messages
    return WndProcDefault(msg, wparam, lparam);
}

// CMDIChildSimple definitions
CMDIChildSimple::CMDIChildSimple()
{
    m_menu.LoadMenu(_T("MdiMenuView"));
    SetHandles(m_menu, NULL);
    SetView(m_view);
}

CMDIChildSimple::~CMDIChildSimple()
{
}

int CMDIChildSimple::OnCreate(CREATESTRUCT& cs)
{
    SetWindowText(_T("Simple Window"));
    SetIconLarge(IDI_VIEW);
    SetIconSmall(IDI_VIEW);

    return CMDIChild::OnCreate(cs);
}

void CMDIChildSimple::OnClose()
{
    int nResult = MessageBox(_T("OK to close Window?"), _T("File Close"), MB_YESNO);

    if (nResult == IDYES)
        MDIDestroy();
}

BOOL CMDIChildSimple::OnCommand(WPARAM wparam, LPARAM lparam)
{
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

LRESULT CMDIChildSimple::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // Do default processing for other messages
    return WndProcDefault(msg, wparam, lparam);
}
