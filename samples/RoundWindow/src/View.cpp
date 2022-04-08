/////////////////////////////
// View.cpp
//

// VS6 requires these macros to be defined.
#define WINVER          0x0410
#define _WIN32_WINDOWS  0x0410
#define _WIN32_IE       0x0401

#include "View.h"
#include "resource.h"

//////////////////////////////
// CView function definitions.
//
CView::CView()
{
    m_brush.CreateSolidBrush( RGB(250, 230, 100) );
}

BOOL CView::OnColor()
{
    static COLORREF custColors[16] = {0};   // Array of custom colors.
    CHOOSECOLOR cc;                         // Structure used by ChooseColor.
    ZeroMemory(&cc, sizeof(cc));

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

    // Set the window title.
    SetWindowText(LoadString(IDW_MAIN));

    // Create a circular region.
    CRgn rgn;
    m_rect = CRect(50, 50, 300, 300);
    rgn.CreateEllipticRgnIndirect(m_rect);

    // Assign the region to the window.
    SetWindowRgn(rgn, FALSE);

    TRACE("OnCreate\n");
    return 0;
}

// Called when the window is destroyed.
void CView::OnDestroy()
{
    // End the application when the window is destroyed.
    ::PostQuitMessage(0);
}

// OnPaint is called automatically whenever a part of the
// window needs to be repainted.
void CView::OnDraw(CDC& dc)
{
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

// This function will be called automatically by Create. It provides an
// opportunity to set various window parameters prior to window creation.
// You are not required to set these parameters, any parameters which
// aren't specified are set to reasonable defaults.
void CView::PreCreate(CREATESTRUCT& cs)
{
    // Set some optional parameters for the window.
    cs.x = 50;                      // top x
    cs.y = 50;                      // top y
    cs.cx = 400;                    // width
    cs.cy = 350;                    // height
    cs.style = WS_VISIBLE | WS_POPUP;          // Window is initially visible.
}

// This function will be called automatically by Create.
// Here we set the window's class parameters.
void CView::PreRegisterClass(WNDCLASS& wc)
{
    wc.hbrBackground = m_brush;                 // Background color
    wc.lpszClassName = _T("Round Window");      // Class name
    wc.hCursor = ::LoadCursor(0,IDC_ARROW);     // Arrow cursor
}

// Respond to a left mouse button press.
LRESULT CView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    SetCapture();

    // Save the relative mouse position.
    m_point = GetCursorPos();
    ScreenToClient(m_point);

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
    {
        CPoint pt = GetCursorPos();

        int x = pt.x - m_point.x;
        int y = pt.y - m_point.y;

        // Move the window.
        SetWindowPos(0, x, y, 0, 0, SWP_NOSIZE);
    }

    // Pass this message on for default processing.
    return FinalWindowProc(msg, wparam, lparam);
}

// Respond to Right mouse button press.
LRESULT CView::OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Create the menu.
    CMenu popup;
    popup.CreatePopupMenu();
    popup.AppendMenu(MF_STRING, IDM_COLOR, _T("Color"));
    popup.AppendMenu(MF_STRING, IDM_EXIT, _T("Exit"));

    // Initiate the popup menu.
    CPoint pt = GetCursorPos();
    popup.TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, pt.x, pt.y, GetHwnd());

    // Pass this message on for default processing.
    return FinalWindowProc(msg, wparam, lparam);
}

// This function is our message procedure. We process the messages for
// the view window here.  Unprocessed messages are passed on for
//  default processing.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_LBUTTONDOWN:    return OnLButtonDown(msg, wparam, lparam);
        case WM_LBUTTONUP:      return OnLButtonUp(msg, wparam, lparam);
        case WM_MOUSEMOVE:      return OnMouseMove(msg, wparam, lparam);
        case WM_RBUTTONDOWN:    return OnRButtonDown(msg, wparam, lparam);
        }

        // Pass unhandled messages on for default processing.
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
