//////////////////////////////////////////////
// SplitterMDIChild.cpp
//  Definitions for the CSimpleView and CSplitterMDIChild classes

#include "stdafx.h"
#include "SplitterMDIChild.h"
#include "Files.h"
#include "Output.h"
#include "resource.h"


// CSimpleView definitions
CSimpleView::CSimpleView() : m_color(RGB(0,0,255))
{
}

void CSimpleView::OnDraw(CDC& dc)
{
    //Centre some text in our view window
    CRect rc = GetClientRect();
    dc.SetTextColor(m_color);
    dc.DrawText(_T("View Window"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CSimpleView::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
// Respond to a mouse click on the window
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CSimpleView::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);
    UNREFERENCED_PARAMETER(lparam);

    Invalidate();   // Force the window to be repainted
    return 0;
}
LRESULT CSimpleView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
    case WM_SIZE:               return OnSize(msg, wparam, lparam);
    }

    // Do default processing for other messages
    return WndProcDefault(msg, wparam, lparam);
}

// CSplitterMDIChild definitions
CSplitterMDIChild::CSplitterMDIChild()
{
    // Set m_View as the view window of the MDI child
    SetView(m_view);
    
    // Set the menu for this MDI child
    SetHandles(LoadMenu(GetApp()->GetResourceHandle(), _T("MdiMenuView")), NULL);
}

CSplitterMDIChild::~CSplitterMDIChild()
{
}

int CSplitterMDIChild::OnCreate(CREATESTRUCT& cs)
{
    // Set the window caption
    SetWindowText( _T("Splitter Window") );
    
    // Set the window icons
    SetIconLarge(IDI_VIEW);
    SetIconSmall(IDI_VIEW);

    return CMDIChild::OnCreate(cs);
}

void CSplitterMDIChild::OnInitialUpdate()
{
    // Add Child dockers
    DWORD style = DS_CLIENTEDGE | DS_NO_UNDOCK;
    m_view.SetDockStyle(style);
    CDocker* pDockLeft  = m_view.AddDockedChild(new CDockOutput, DS_DOCKED_LEFT  | style, 200, 0);
    CDocker* pDockRight = m_view.AddDockedChild(new CDockOutput, DS_DOCKED_RIGHT | style, 200, 0);
    pDockLeft->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | style, 0, 0);
    pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | style, 0, 0);    
}

BOOL CSplitterMDIChild::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // Respond to menu and toolbar input

    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDM_COLOR_BLACK:   OnColor(RGB(0,0,0));        return TRUE;
    case IDM_COLOR_RED:     OnColor(RGB(255,0,0));      return TRUE;
    case IDM_COLOR_GREEN:   OnColor(RGB(0,255,0));      return TRUE;
    case IDM_COLOR_BLUE:    OnColor(RGB(0,0,255));      return TRUE;
    case IDM_COLOR_WHITE:   OnColor(RGB(255,255,255));  return TRUE;
    } 

    return FALSE;
}

void CSplitterMDIChild::OnColor(COLORREF rgb)
{
    m_view.GetSimpleView().SetColor(rgb);
    m_view.GetSimpleView().Invalidate();
}

LRESULT CSplitterMDIChild::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    m_view.RecalcDockLayout();

    // Pass the message on for default processing
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CSplitterMDIChild::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_SIZE:   return OnSize(msg, wparam, lparam);
    }

    // Do default processing for other messages
    return WndProcDefault(msg, wparam, lparam);
}
