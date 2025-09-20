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
    m_menu.LoadMenu(L"MdiMenuMax");
    SetHandles(m_menu, 0);
    SetView(m_maxView);
}

// Called when the window is created.
int CMDIChildMax::OnCreate(CREATESTRUCT& cs)
{
    SetWindowText(L"Maximised Window");
    SetIconLarge(IDI_MAX);
    SetIconSmall(IDI_MAX);

    return CMDIChild::OnCreate(cs);
}

// Set the CREATESTRUCT parameters before the window is created.
void CMDIChildMax::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_MAXIMIZE;
}

// Handle the window's messages.
LRESULT CMDIChildMax::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
    dc.DrawText(L"Maxed Window", -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
