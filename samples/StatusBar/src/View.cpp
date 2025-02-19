/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"

//////////////////////////////
// CView function definitions.
//

void CView::OnDraw(CDC& dc)
// OnDraw is called when part or all of the window needs to be redrawn.
{
    // Use the message font for Windows 7 and higher.
    if (GetWinVersion() >= 2601)
    {
        NONCLIENTMETRICS info = GetNonClientMetrics();
        LOGFONT lf = DpiScaleLogfont(info.lfMessageFont, 10);
        dc.CreateFontIndirect(lf);
    }

    // Centre some text in our view window.
    CRect rc = GetClientRect();
    dc.DrawText(L"View Window", -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CView::OnInitialUpdate()
// OnInitialUpdate is called immediately after the window is created.
{
    TRACE("View window created\n");
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // Here we set the defaults used by the create function for the view window
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the window we create.

    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS& wc)
{
    // Here we set the Window class parameters.
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the type of window we create.

    // Set the window class name.
    wc.lpszClassName = L"Win32++ View";

    // Set a background brush to white.
    wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

    // Set the default cursor.
    wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);

    // Set the class style (not to be confused with the window styles set in PreCreate).
    wc.style = CS_DBLCLKS;  // Generate left button double click messages.
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
// All window messages for this window pass through WndProc.
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:
            Invalidate();
            break;  // Also do default processing.
        }

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
