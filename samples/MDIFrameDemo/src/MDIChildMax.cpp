//////////////////////////////////////////////
// MDIChildMax.cpp
//  Definitions for the CViewMax and CMDIChildMax classes

#include "stdafx.h"
#include "MDIChildMax.h"
#include "resource.h"


////////////////////////////////////
// CMDIChildMax function definitions
//

// Constructor.
CMDIChildMax::CMDIChildMax()
{
    m_menu.LoadMenu(_T("MdiMenuMax"));
    SetHandles(m_menu, 0);
    SetView(m_maxView);
}

// Destructor.
CMDIChildMax::~CMDIChildMax()
{
}

// Called when the window is created.
int CMDIChildMax::OnCreate(CREATESTRUCT& cs)
{
    SetWindowText(_T("Maximised Window"));
    SetIconLarge(IDI_MAX);
    SetIconSmall(IDI_MAX);

    return CMDIChild::OnCreate(cs);
}

// Set the CREATESTRUCT parameters before the window is created.
void CMDIChildMax::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_MAXIMIZE;
}


////////////////////////////////
// CViewMax function definitions
//

// Constructor.
void CViewMax::OnDraw(CDC& dc)
{
    // Use the message font for Windows 7 and higher.
    if (GetWinVersion() >= 2601)
    {
        NONCLIENTMETRICS info = GetNonClientMetrics();
        LOGFONT lf = info.lfMessageFont;
        dc.CreateFontIndirect(lf);
    }

    // Centre some text in our view window.
    CRect rc = GetClientRect();
    dc.DrawText(_T("Maxed Window"), -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// Process the max view's window messages.
LRESULT CViewMax::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:
            Invalidate();
            return 0;

        }

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
