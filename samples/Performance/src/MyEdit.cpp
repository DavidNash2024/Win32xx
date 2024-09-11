/////////////////////////////
// CMyEdit.cpp
//

#include "stdafx.h"
#include "MyEdit.h"

///////////////////////////////
// CMyEdit function definitions
//

// Constructor.
CMyEdit::CMyEdit()
{
}

void CMyEdit::DPISetFont()
{
    m_font.CreatePointFont(100, L"Courier New");
    m_font = DpiScaleFont(m_font, 10);
    SetFont(m_font);
}

// Called when a window handle (HWND) is attached to CMyEdit.
void CMyEdit::OnAttach()
{
    // Set the font for the Edit window
    DPISetFont();
}

// Set the CREATESTRUCT parameters before the window is created.
void CMyEdit::PreCreate(CREATESTRUCT& cs)
{
    // Specify the edit window's style before the window is created
    cs.style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL;
}

// Handle the window's messages.
LRESULT CMyEdit::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
