//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"


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
    clientDC.MoveTo(m_OldPt.x, m_OldPt.y);
    clientDC.LineTo(x, y);
}


// Called when the window is destroyed.
void CView::OnDestroy()
{
    //End the program when window is destroyed
    ::PostQuitMessage(0);
}


// Called when the left mouse button is pressed while the cursor is over the window.
LRESULT CView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Capture mouse input.
    SetCapture();

    m_OldPt.x = GET_X_LPARAM(lparam);
    m_OldPt.y = GET_Y_LPARAM(lparam);

    return FinalWindowProc(msg, wparam, lparam);
}


// Called when the left mouse button is released.
LRESULT CView::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    //Release the capture on the mouse
    ReleaseCapture();

    return FinalWindowProc(msg, wparam, lparam);
}


// Called when the mouse is moved while captured.
LRESULT CView::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // hold down the left mouse button and move mouse to draw lines.
    if ( (wparam & MK_LBUTTON) && (GetCapture() == *this) )
    {
        DrawLine(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
        m_OldPt.x = GET_X_LPARAM(lparam);
        m_OldPt.y = GET_Y_LPARAM(lparam);
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

