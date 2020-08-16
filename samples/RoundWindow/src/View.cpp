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
}

BOOL CView::OnColor()
{
    static COLORREF custColors[16] = {0};   // array of custom colors
    CHOOSECOLOR cc;                         // Structure used by ChooseColor
    ZeroMemory(&cc, sizeof(cc));

    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    cc.rgbResult = m_brush.GetLogBrush().lbColor;
    cc.lpCustColors = custColors;
    cc.hwndOwner = *this;

    // Initiate the Choose Color dialog
    if (ChooseColor(&cc)==TRUE)
    {
        m_brush = CreateSolidBrush(cc.rgbResult);
        SetClassLongPtr(GCLP_HBRBACKGROUND, (LONG_PTR)m_brush.GetHandle());
        Invalidate();
    }

    return TRUE;
}

BOOL CView::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id= LOWORD(wparam);
    switch (id)
    {
    case IDM_COLOR:     return OnColor();
    case IDM_EXIT:      return OnExit();
    }

    // return FALSE for unhandled commands
    return FALSE;
}

int CView::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate is called automatically during window creation when a
    // WM_CREATE message received.

    // Tasks such as setting the icon, creating child windows, or anything
    // associated with creating windows are normally performed here.

    UNREFERENCED_PARAMETER(cs);

    // Set the window's icon
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Set the window title
    SetWindowText(LoadString(IDW_MAIN));

    // Create a circular region
    CRgn rgn;
    m_rect = CRect(50, 50, 300, 300);
    rgn.CreateEllipticRgnIndirect(m_rect);

    // assign the region to the window
    SetWindowRgn(rgn, FALSE);

    TRACE("OnCreate\n");
    return 0;
}

void CView::OnDestroy()
{
    // End the application when the window is destroyed
    ::PostQuitMessage(0);
}

void CView::OnDraw(CDC& dc)
{
    // OnPaint is called automatically whenever a part of the
    // window needs to be repainted.

    // Centre some text in our view window
    CRect rc = m_rect;
    CString cs = LoadString(IDW_MAIN);
    dc.SetBkMode(TRANSPARENT);
    dc.DrawText(cs, cs.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

BOOL CView::OnExit()
{
    // Issue a close request to the window.
    Close();
    return TRUE;
}

void CView::OnInitialUpdate()
{
    // OnInitialUpdate is called after the window is created.
    // Tasks which are to be done after the window is created go here.

    CenterWindow();
    TRACE("OnInitialUpdate\n");
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // This function will be called automatically by Create. It provides an
    // opportunity to set various window parameters prior to window creation.
    // You are not required to set these parameters, any paramters which
    // aren't specified are set to reasonable defaults.

    // Set some optional parameters for the window
    cs.x = 50;                      // top x
    cs.y = 50;                      // top y
    cs.cx = 400;                    // width
    cs.cy = 350;                    // height
    cs.style = WS_VISIBLE | WS_POPUP;          // Window is initially visible
}

void CView::PreRegisterClass(WNDCLASS& wc)
{
    // This function will be called automatically by Create.
    // Here we set the window's class paramaters.

    wc.hbrBackground = m_brush;                 // Background color
    wc.lpszClassName = _T("Round Window");      // Class Name
    wc.hCursor = ::LoadCursor(NULL,IDC_ARROW);  // Arrow cursor
}

LRESULT CView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
// Respond to a left mouse button press
{
    SetCapture();

    // Save the relative mouse position
    m_point = GetCursorPos();
    ScreenToClient(m_point);

    // Pass this message on for default processing
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
// Respond to a left mouse button release
{
    ReleaseCapture();

    // Pass this message on for default processing
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
// Respond to a mouse move
{
    if (wparam & MK_LBUTTON)
    {
        CPoint pt = GetCursorPos();

        int x = pt.x - m_point.x;
        int y = pt.y - m_point.y;

        // Move the window
        SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE);
    }

    // Pass this message on for default processing
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
// Respond to Right mouse button press
{
    // Create the menu
    CMenu popup;
    popup.CreatePopupMenu();
    popup.AppendMenu(MF_BYPOSITION | MF_STRING, IDM_COLOR, _T("Color"));
    popup.AppendMenu(MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));

    // Initiate the popup menu
    CPoint pt = GetCursorPos();
    popup.TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, pt.x, pt.y, GetHwnd());

    // Pass this message on for default processing
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // This function is our message procedure. We process the messages for
    // the view window here.  Unprocessed messages are passed on for
    //  default processing.

    switch (msg)
    {
    case WM_LBUTTONDOWN:    return OnLButtonDown(msg, wparam, lparam);
    case WM_LBUTTONUP:      return OnLButtonUp(msg, wparam, lparam);
    case WM_MOUSEMOVE:      return OnMouseMove(msg, wparam, lparam);
    case WM_RBUTTONDOWN:    return OnRButtonDown(msg, wparam, lparam);
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}
