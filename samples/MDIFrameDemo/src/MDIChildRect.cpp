//////////////////////////////////////////////
// MDIChildRect.cpp
//  Definitions for the CViewRect and CMDIChildRect class

#include "stdafx.h"
#include "MDIChildRect.h"
#include "resource.h"


/////////////////////////////////
// CViewRect function definitions
//

// Called when the window is created
int CViewRect::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);
    SetTimer (1, 250, NULL) ;
    return 0;
}

// Called when the window is destroyed.
void CViewRect::OnDestroy()
{
    KillTimer(1);
}

// Called when the window is resized.
LRESULT CViewRect::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);

    if (wparam != SIZE_MINIMIZED)
    {
        m_cxClientMax = LOWORD (lparam) ;
        if (m_cxClientMax < 1)
            m_cxClientMax = 1;

        m_cyClientMax = HIWORD (lparam) ;
        if (m_cyClientMax < 1)
            m_cyClientMax = 1;
    }

    return 0;
}

// Called when a timer event occurs.
LRESULT CViewRect::OnTimer(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);
    UNREFERENCED_PARAMETER(lparam);

    int red, green, blue;
    int xLeft, xRight, yTop, yBottom;
    xLeft   = rand () % m_cxClientMax;
    xRight  = rand () % m_cxClientMax;
    yTop    = rand () % m_cyClientMax;
    yBottom = rand () % m_cyClientMax;
    red    = rand () & 255;
    green  = rand () & 255;
    blue   = rand () & 255;

    CClientDC dcRect(*this);
    dcRect.CreateSolidBrush (RGB (red, green, blue));

    int rcLeft   = (xLeft < xRight) ? xLeft : xRight;
    int rcTop    = (yTop < yBottom) ? yTop  : yBottom;
    int rcRight  = (xLeft > xRight) ? xLeft : xRight;
    int rcBottom = (yTop > yBottom) ? yTop  : yBottom;
    dcRect.Rectangle(rcLeft, rcTop, rcRight, rcBottom);

    return 0;
}

// Process the view rect's window messages.
LRESULT CViewRect::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_TIMER:  return OnTimer(msg, wparam, lparam);       // Display a random rectangle
    case WM_SIZE:   return OnSize(msg, wparam, lparam);        // If not minimized, save the window size
    }

    return WndProcDefault(msg, wparam, lparam);
}

/////////////////////////////////////
// CMDIChildRect function definitions
//

// Constructor.
CMDIChildRect::CMDIChildRect()
{
    m_menu.LoadMenu(_T("MdiMenuRect"));
    SetHandles(m_menu, 0);
    SetView(m_rectView);
}

// Destructor.
CMDIChildRect::~CMDIChildRect()
{
}

// Called when the window is created.
int CMDIChildRect::OnCreate(CREATESTRUCT& cs)
{
    SetWindowText(_T("Rectangle Window"));
    SetIconLarge(IDI_RECT);
    SetIconSmall(IDI_RECT);

    return CMDIChild::OnCreate(cs);
}

