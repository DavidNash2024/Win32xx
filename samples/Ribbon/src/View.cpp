/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"
#include "Resource.h"
#include "RibbonUI.h"

//////////////////////////////
// CView function definitions.
//

// Executes commands in response the the ribbon's buttons.
STDMETHODIMP CView::Execute(UINT32 cmdID, UI_EXECUTIONVERB, const PROPERTYKEY*, const PROPVARIANT*, IUISimplePropertySet*)
{
    HRESULT result = S_OK;

    switch (cmdID)
    {
    case cmdButton1:
        TRACE(L"Button 1\n");
        break;
    case cmdButton2:
        TRACE(L"Button 2\n");
        break;
    case cmdButton3:
        TRACE(L"Button 3\n");
        break;
    case cmdButton4:
        TRACE(L"Button 4\n");
        break;
    case cmdButton5:
        TRACE(L"Button 5\n");
        break;
    case cmdButton6:
        TRACE(L"Button 6\n");
        break;
    case cmdToggleButton1:
        TRACE(L"Toggle Button 1\n");
        break;
    case cmdToggleButton2:
        TRACE(L"Toggle Button 2\n");
        break;
    case IDC_CMD_EXIT:
        TRACE(L"Exit button\n");
        Close();
        break;
    default:
        TRACE(L"Unknown button\n");
        break;
    }

    return result;
}

// The IUIRibbon interface provides the ability to specify settings and properties for the ribbon.
IUIRibbon* CView::GetIUIRibbon() const
{
    return m_pIUIRibbon;
}

// OnCreate is called automatically during window creation when a
// WM_CREATE message received.
int CView::OnCreate(CREATESTRUCT&)
{
    // Tasks such as setting the icon, creating child windows, or anything
    // associated with creating windows are normally performed here.

    TRACE(L"OnCreate\n");

    // Set the window's icon.
    SetIconSmall(IDI_SMALL);
    SetIconLarge(IDI_SIMPLERIBBON);

    // Set the window title.
    SetWindowText(LoadString(IDS_APP_TITLE).c_str());

    if (GetWinVersion() >= 2601)        // Ribbon is only supported on Windows 7 and above.
    {
        if (SUCCEEDED(CreateRibbon(*this)))
            TRACE(L"Ribbon Created Successfully\n");
        else
            TRACE(L"Failed to create ribbon\n");
    }

    return 0;
}

// OnDestroy is called when the view window is destroyed.
void CView::OnDestroy()
{
    if (GetWinVersion() >= 2601)        // Ribbon only supported on Windows 7 and above
    {
        DestroyRibbon();
    }

    // End the application when the window is destroyed
    ::PostQuitMessage(0);
}

// Called when the effective dots per inch (dpi) for a window has changed.
// This occurs when:
//  - The window is moved to a new monitor that has a different DPI.
//  - The DPI of the monitor hosting the window changes.
LRESULT CView::OnDpiChanged(UINT, WPARAM, LPARAM lparam)
{
    LPRECT prc = reinterpret_cast<LPRECT>(lparam);
    SetWindowPos(HWND_TOP, *prc, SWP_SHOWWINDOW);

    return 0;
}

// OnPaint is called automatically whenever a part of the
// window needs to be repainted.
void CView::OnDraw(CDC& dc)
{
    // Use the message font for Windows 7 and higher.
    if (GetWinVersion() >= 2601)
    {
        NONCLIENTMETRICS info = GetNonClientMetrics();
        LOGFONT lf = info.lfMessageFont;
        int dpi = GetWindowDpi(*this);
        lf.lfHeight = -MulDiv(10, dpi, POINTS_PER_INCH);
        dc.CreateFontIndirect(lf);
    }

    // Centre some text in our view window
    CRect r = GetClientRect();
    r.top += GetRibbonHeight();
    dc.DrawText(L"Simple Ribbon Demo", -1, r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

// OnInitialUpdate is called after the window is created.
void CView::OnInitialUpdate()
{
    // Tasks which are to be done after the window is created go here.

    RedrawWindow();
    TRACE(L"OnInitialUpdate\n");
}

// OnSize is called when the window is resized.
LRESULT CView::OnSize()
{
    RecalcLayout();
    return 0;
}

// OnViewChanged is called when the ribbon has changed.
STDMETHODIMP CView::OnViewChanged(UINT32, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb, INT32)
{
    HRESULT result = E_NOTIMPL;

    // Checks to see if the view that was changed was a Ribbon view.
    if (UI_VIEWTYPE_RIBBON == typeId)
    {
        switch (verb)
        {
        case UI_VIEWVERB_CREATE:    // The ribbon has been created.
            m_pIUIRibbon = reinterpret_cast<IUIRibbon*>(pView);
            result = S_OK;
            break;
        case UI_VIEWVERB_SIZE:      // The ribbon's size has changed
            RecalcLayout();
            result = S_OK;
            break;
        case UI_VIEWVERB_DESTROY:   // The ribbon has been destroyed.
            m_pIUIRibbon = nullptr;
            result = S_OK;
            break;
        case UI_VIEWVERB_ERROR:
            result = E_FAIL;
            break;
        }
    }

    return result;
}

// PreCreate is called before the window is created.
void CView::PreCreate(CREATESTRUCT& cs)
{
    // This function will be called automatically by Create. It provides an
    // opportunity to set various window parameters prior to window creation.
    // You are not required to set these parameters, any paramters which
    // aren't specified are set to reasonable defaults.

    // Set some optional parameters for the window
    cs.dwExStyle = WS_EX_CLIENTEDGE;        // Extended style
    cs.lpszClass = L"View Window";          // Window Class
    cs.x = DpiScaleInt(50);                 // top x
    cs.y = DpiScaleInt(50);                 // top y
    cs.cx = DpiScaleInt(600);               // width
    cs.cy = DpiScaleInt(500);               // height
}

// Recalculate the position of the child windows.
void CView::RecalcLayout()
{
    CRect r = GetClientRect();
    r.top += GetRibbonHeight();
    InvalidateRect(r);
}

// The view's window procedure handles the window's messages.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DPICHANGED:  return OnDpiChanged(msg, wparam, lparam);
        case WM_SIZE:        return OnSize();
        }

        // pass unhandled messages on for default processing
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
