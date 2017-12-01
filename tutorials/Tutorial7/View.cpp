//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "targetver.h"
#include "view.h"
#include "resource.h"


CView::CView() : m_PenColor(RGB(0,0,0))
{
    m_Brush.CreateSolidBrush(RGB(255,255,230));
}

CView::~CView()
{
}

void CView::DrawLine(int x, int y)
// Draws a line in the window's client area
{
    CClientDC clientDC(*this);
    clientDC.CreatePen(PS_SOLID, 1, m_points.back().color);
    clientDC.MoveTo(m_points.back().x, m_points.back().y);
    clientDC.LineTo(x, y);
}

void CView::OnDraw(CDC& dc)
// Called when drawing to the window
{
    if (m_points.size() > 0)
    {
        bool isDrawing = false;  //Start with the pen up
        for (unsigned int i = 0 ; i < m_points.size(); i++)
        {
            dc.CreatePen(PS_SOLID, 1, m_points[i].color);
            if (isDrawing)
                dc.LineTo(m_points[i].x, m_points[i].y);
            else
                dc.MoveTo(m_points[i].x, m_points[i].y);
            
            isDrawing = m_points[i].PenDown;
        }
    }
}

void CView::StorePoint(int x, int y, bool PenDown)
{
    PlotPoint pp;
    pp.x = x;
    pp.y = y;
    pp.PenDown = PenDown;
    pp.color = m_PenColor;

    m_points.push_back(pp); //Add the point to the vector
}

LRESULT CView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called when the left mouse button is pressed while the cursor is over the window.
{
    // Capture mouse input.
    SetCapture();
    StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);

    return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called when the left mouse button is released
{
    //Release the capture on the mouse
    ReleaseCapture();
    StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);

    return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called when the mouse is moved while captured
{
    // hold down the left mouse button and move mouse to draw lines.
    if ( (wParam & MK_LBUTTON) && (GetCapture() == *this) )
    {
        DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
    }
    
    return FinalWindowProc(uMsg, wParam, lParam);
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
    wc.hbrBackground = m_Brush;
    wc.lpszClassName = _T("Scribble Window");
    wc.hCursor = GetApp().LoadCursor(IDC_CURSOR1);
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called to handle the window's messages
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:    return OnLButtonDown(uMsg, wParam, lParam);
    case WM_MOUSEMOVE:      return OnMouseMove(uMsg, wParam, lParam);
    case WM_LBUTTONUP:      return OnLButtonUp(uMsg, wParam, lParam);   
    }

    //Use the default message handling for remaining messages
    return WndProcDefault(uMsg, wParam, lParam);
}

