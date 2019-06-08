//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"
#include "resource.h"


CView::CView()
{
}


CView::~CView()
{
}


// Draws a line in the window's client area.
void CView::DrawLine(int x, int y)
{
    CClientDC clientDC(*this);
    clientDC.MoveTo(m_points.back().x, m_points.back().y);
    clientDC.LineTo(x, y);
}


// Called when drawing to the window.
void CView::OnDraw(CDC& dc)
{
    if (m_points.size() > 0)
    {
        bool isPenDown = false;  //Start with the pen up

        for (unsigned int i = 0 ; i < m_points.size(); i++)
        {
            if (isPenDown)
                dc.LineTo(m_points[i].x, m_points[i].y);
            else
                dc.MoveTo(m_points[i].x, m_points[i].y);
            
            isPenDown = m_points[i].isPenDown;
        }
    }
}


// Store the specified point information.
void CView::StorePoint(int x, int y, bool isPenDown)
{
    PlotPoint pp;
    pp.x = x;
    pp.y = y;
    pp.isPenDown = isPenDown;

    m_points.push_back(pp); //Add the point to the vector
}


// Called when the left mouse button is pressed while the cursor is over the window.
LRESULT CView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Capture mouse input.
    SetCapture();
    StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), true);

    return FinalWindowProc(msg, wparam, lparam);
}


// Called when the left mouse button is released.
LRESULT CView::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    //Release the capture on the mouse
    ReleaseCapture();
    StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), false);

    return FinalWindowProc(msg, wparam, lparam);
}


// Called when the mouse is moved while captured.
LRESULT CView::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // hold down the left mouse button and move mouse to draw lines.
    if ( (wparam & MK_LBUTTON) && (GetCapture() == *this) )
    {
        DrawLine(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
        StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), true);
    }
    
    return FinalWindowProc(msg, wparam, lparam);
}


// Called to handle the window's messages.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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

