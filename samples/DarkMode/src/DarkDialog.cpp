/////////////////////////////
// Help.cpp
//

#include "stdafx.h"
#include "DarkDialog.h"
#include "resource.h"

///////////////////////////////////
// CDarkDialog function definitions
//

// Constructor.
CDarkDialog::CDarkDialog() : m_darkMode(false)
{
    SetDialogFromID(IDW_ABOUT);
    m_blackBrush.CreateSolidBrush(RGB(0, 0, 0));
}

// Destructor.
CDarkDialog::~CDarkDialog()
{
}

// Process the dialog's window messages.
INT_PTR CDarkDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_CTLCOLORDLG:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORSTATIC:  return OnCtlColors(msg, wparam, lparam);
        case WM_DRAWITEM:        return OnDrawItem(wparam, lparam);
        }

        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    // catch all CException types
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

LRESULT CDarkDialog::OnCtlColors(UINT, WPARAM wparam, LPARAM)
{
    if (m_darkMode)
    {
        HDC dc = (HDC)wparam;
        ::SetBkMode(dc, TRANSPARENT);
        ::SetTextColor(dc, RGB(255, 255, 255));

        static CBrush brush(RGB(0, 0, 0));
        return reinterpret_cast<LRESULT>(brush.GetHandle());
    }

    return 0;
}

LRESULT CDarkDialog::OnDrawItem(WPARAM, LPARAM lparam)
{
    LPDRAWITEMSTRUCT pDraw = (LPDRAWITEMSTRUCT)lparam;
    CDC dc(pDraw->hDC);
    CRect rect = pDraw->rcItem;
    dc.SetTextColor(RGB(220, 220, 220));

    if (pDraw->itemState & ODS_SELECTED)
    {
        dc.CreatePen(PS_SOLID, 1, RGB(140, 140, 140));
        dc.CreateSolidBrush(RGB(60, 60, 60));
    }
    else
    {
        dc.CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
        dc.CreateSolidBrush(RGB(80, 80, 80));
    }

    dc.RoundRect(rect, 10, 10);

    // Draw the button text.
    dc.DrawText(GetWindowText(), -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    return TRUE;
}

// Called before the dialog is displayed.
BOOL CDarkDialog::OnInitDialog()
{
    // Set the application icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Make the OK button owner drawn for dark mode.
    SetButtonOwnerDraw(m_darkMode);

    return TRUE;
}

void CDarkDialog::SetButtonOwnerDraw(bool isOwnerDraw)
{
    HWND ok = ::GetDlgItem(*this, IDOK);
    DWORD style1 = static_cast<DWORD>(::GetWindowLongPtr(ok, GWL_STYLE));

    if (isOwnerDraw)
        ::SetWindowLongPtr(ok, GWL_STYLE, style1 | BS_OWNERDRAW);
    else
        ::SetWindowLongPtr(ok, GWL_STYLE, style1 & ~BS_OWNERDRAW);
}

void CDarkDialog::SetDarkMode(bool darkMode)
{
    // Make the OK button owner drawn for dark mode.
    if (IsWindow())
        SetButtonOwnerDraw(darkMode);

    m_darkMode = darkMode;
}

