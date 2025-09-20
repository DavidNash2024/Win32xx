/////////////////////////////
// View.cpp
//

#include "View.h"
#include "resource.h"


//////////////////////////////
// CView function definitions.
//
CView::CView()
{
    m_brush.CreateSolidBrush( RGB(250, 230, 100) );

    // Set the old DPI to the desktop window's DPI.
    m_oldDPI = GetWindowDpi(HWND_DESKTOP);
}

BOOL CView::OnColor()
{
    static COLORREF custColors[16] = {0};   // Array of custom colors.
    CHOOSECOLOR cc{};                       // Structure used by ChooseColor.
    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    cc.rgbResult = m_brush.GetLogBrush().lbColor;
    cc.lpCustColors = custColors;
    cc.hwndOwner = *this;

    // Initiate the Choose Color dialog.
    if (ChooseColor(&cc)==TRUE)
    {
        m_brush.CreateSolidBrush(cc.rgbResult);
        SetClassLongPtr(GCLP_HBRBACKGROUND, (LONG_PTR)m_brush.GetHandle());
        Invalidate();
    }

    return TRUE;
}

// Respond to popup menu input.
BOOL CView::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id= LOWORD(wparam);

    switch (id)
    {
    case IDM_COLOR:     return OnColor();
    case IDM_EXIT:      return OnExit();
    }

    // Return FALSE for unhandled commands.
    return FALSE;
}

// OnCreate is called automatically during window creation when a
// WM_CREATE message received.
// Tasks such as setting the icon, creating child windows, or anything
// associated with creating windows are normally performed here.
int CView::OnCreate(CREATESTRUCT&)
{
    // Set the window's icon
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);


    // Set the window's region.
    SetRoundRegion();

    TRACE("OnCreate\n");
    return 0;
}

// Called when the window is destroyed.
void CView::OnDestroy()
{
    // End the application when the window is destroyed.
    ::PostQuitMessage(0);
}

// Called in response to a WM_DPICHANGED message that is sent to a top-level
// window when the DPI changes. Only top-level windows receive a WM_DPICHANGED message.
LRESULT CView::OnDpiChanged(UINT, WPARAM, LPARAM lparam)
{
    // Resize the window.
    RECT* const pWindowRect = reinterpret_cast<RECT*>(lparam);
    assert(pWindowRect);
    SetWindowPos(HWND_TOP, *pWindowRect, SWP_NOZORDER | SWP_NOACTIVATE);
    SetRoundRegion();

    // Update the grap point for the new DPI.
    int newDPI = GetWindowDpi(*this);
    m_grabPoint.x = m_grabPoint.x * newDPI / m_oldDPI;
    m_grabPoint.y = m_grabPoint.y * newDPI / m_oldDPI;
    m_oldDPI = newDPI;

    // Position the window under the cursor.
    PositionWindow();

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
        LOGFONT lf = DpiScaleLogfont(info.lfMessageFont, 10);
        dc.CreateFontIndirect(lf);
    }

    // Centre some text in our view window.
    CRect rc = m_rect;
    CString cs = LoadString(IDW_MAIN);
    dc.SetBkMode(TRANSPARENT);
    dc.DrawText(cs, cs.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

// Called from menu input.
BOOL CView::OnExit()
{
    // Issue a close request to the window.
    Close();
    return TRUE;
}

// OnInitialUpdate is called after the window is created.
// Tasks which are to be done after the window is created go here.
void CView::OnInitialUpdate()
{
    CenterWindow();
    TRACE("OnInitialUpdate\n");
}

// Respond to a left mouse button press.
LRESULT CView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Capture the mouse input.
    SetCapture();

    // Save the relative mouse position.
    m_grabPoint = GetCursorPos();
    ScreenToClient(m_grabPoint);

    // Pass this message on for default processing.
    return FinalWindowProc(msg, wparam, lparam);
}

// Respond to a left mouse button release.
LRESULT CView::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    ReleaseCapture();

    // Pass this message on for default processing.
    return FinalWindowProc(msg, wparam, lparam);
}

// Respond to a mouse move.
LRESULT CView::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (wparam & MK_LBUTTON)
        PositionWindow();

    // Pass this message on for default processing.
    return FinalWindowProc(msg, wparam, lparam);
}

// Respond to right mouse button press.
LRESULT CView::OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Create the menu.
    CMenu popup;
    popup.CreatePopupMenu();
    popup.AppendMenu(MF_STRING, IDM_COLOR, L"Color");
    popup.AppendMenu(MF_STRING, IDM_EXIT, L"Exit");

    // Initiate the popup menu.
    CPoint pt = GetCursorPos();
    popup.TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, pt.x, pt.y, GetHwnd());

    // Pass this message on for default processing.
    return FinalWindowProc(msg, wparam, lparam);
}

void CView::PositionWindow()
{
    CPoint pt = GetCursorPos();

    int x = pt.x - m_grabPoint.x;
    int y = pt.y - m_grabPoint.y;

    // Move the window.
    SetWindowPos(HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
}

// This function will be called automatically by Create. It provides an
// opportunity to set various window parameters prior to window creation.
// You are not required to set these parameters, any parameters which
// aren't specified are set to reasonable defaults.
void CView::PreCreate(CREATESTRUCT& cs)
{
    // Set some optional parameters for the window.
    cs.x = DpiScaleInt(50);                    // top x
    cs.y = DpiScaleInt(50);                    // top y
    cs.cx = DpiScaleInt(400);                  // width
    cs.cy = DpiScaleInt(350);                  // height
    cs.style = WS_VISIBLE | WS_POPUP;          // Window is initially visible.
}

// This function will be called automatically by Create.
// Here we set the window's class parameters.
void CView::PreRegisterClass(WNDCLASS& wc)
{
    wc.hbrBackground = m_brush;                 // Background color
    wc.lpszClassName = L"Round Window";      // Class name
    wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);     // Arrow cursor
}

void CView::SetRoundRegion()
{
    SetWindowRgn(nullptr, TRUE);

    // Create a circular region.
    CRgn rgn;
    int left = DpiScaleInt(50);
    int top = DpiScaleInt(50);
    int right = DpiScaleInt(300);
    int bottom = DpiScaleInt(300);
    m_rect = CRect(left, top, right, bottom);
    rgn.CreateEllipticRgnIndirect(m_rect);

    // Assign the region to the window.
    SetWindowRgn(rgn, FALSE);
}

// This function is our message procedure. We process the messages for
// the view window here.  Unprocessed messages are passed on for
// default processing.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DPICHANGED:     return OnDpiChanged(msg, wparam, lparam);
        case WM_LBUTTONDOWN:    return OnLButtonDown(msg, wparam, lparam);
        case WM_LBUTTONUP:      return OnLButtonUp(msg, wparam, lparam);
        case WM_MOUSEMOVE:      return OnMouseMove(msg, wparam, lparam);
        case WM_RBUTTONDOWN:    return OnRButtonDown(msg, wparam, lparam);
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
