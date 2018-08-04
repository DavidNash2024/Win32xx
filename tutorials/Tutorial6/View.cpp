//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class


#include "view.h"
#include "resource.h"


CView::CView()
{
    m_brush.CreateSolidBrush(RGB(255,255,230));
}

CView::~CView()
{
}

void CView::DrawLine(int x, int y)
// Draws a line in the window's client area
{
    CClientDC clientDC(*this);
	clientDC.MoveTo(m_points.back().x, m_points.back().y);
	clientDC.LineTo(x, y);
}

void CView::OnDraw(CDC& dc)
// Called when drawing to the window
{
    if (m_points.size() > 0)
    {
        bool isPenDown = false;  //Start with the pen up
        for (size_t i = 0 ; i < m_points.size(); i++)
        {
            if (isPenDown)
                dc.LineTo(m_points[i].x, m_points[i].y);
            else
                dc.MoveTo(m_points[i].x, m_points[i].y);
            
			isPenDown = m_points[i].isPenDown;
        }
    }
}

void CView::StorePoint(int x, int y, bool isPenDown)
{
    PlotPoint pp;
    pp.x = x;
    pp.y = y;
    pp.isPenDown = isPenDown;

    m_points.push_back(pp); //Add the point to the vector
}

LRESULT CView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
// Called when the left mouse button is pressed while the cursor is over the window.
{
    // Capture mouse input.
    SetCapture();
    StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), true);

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
// Called when the left mouse button is released
{
    //Release the capture on the mouse
    ReleaseCapture();
    StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), false);

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
// Called when the mouse is moved while captured
{
    // hold down the left mouse button and move mouse to draw lines.
    if ( (wparam & MK_LBUTTON) && (GetCapture() == *this) )
    {
        DrawLine(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
        StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), true);
    }
    
    return FinalWindowProc(msg, wparam, lparam);
}

void CView::PreCreate(CREATESTRUCT& cs)
// Called before window creation to update the window's CREATESTRUCT
{
    // Set the extra style to provide a sunken effect
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS& wc)
// Called before the window is registered to update the window's WNDCLASS
{
    // Set the background brush, class name and cursor
    wc.hbrBackground = m_brush;
    wc.lpszClassName = _T("Scribble Window");
    wc.hCursor = GetApp().LoadCursor(IDC_CURSOR1);
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
// Called to handle the window's messages
{
    switch (msg)
    {
    case WM_LBUTTONDOWN:    return OnLButtonDown(msg, wparam, lparam);
    case WM_MOUSEMOVE:      return OnMouseMove(msg, wparam, lparam);
    case WM_LBUTTONUP:      return OnLButtonUp(msg, wparam, lparam);
    }

    //Use the default message handling for remaining messages
    return WndProcDefault(msg, wparam, lparam);
}

