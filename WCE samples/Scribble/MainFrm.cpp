#include "MainFrm.h"
#include "resource.h"



CMainFrame::CMainFrame() : m_penColor(RGB(0,0,0))
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( 0 );          // Separator
    AddToolBarButton( IDM_RED  );
    AddToolBarButton( IDM_BLUE );
    AddToolBarButton( IDM_GREEN );
    AddToolBarButton( IDM_BLACK );
}

void CMainFrame::DrawLine(short x, short y)
{
    CDC dc = GetDC();
    dc.CreatePen(PS_SOLID, 1, m_points.back().penColor);
    dc.MoveTo(m_points.back().x, m_points.back().y);
    dc.LineTo(x, y);
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM /*lparam*/)
{
    // Respond to menu and toolbar selections
    switch (LOWORD(wparam))
    {
    // Respond to menu items
    case IDM_NEW:
        m_points.clear();
        Invalidate();
        return TRUE;
    case IDM_HELP_ABOUT:
        {
            CDialog HelpDialog(IDW_ABOUT);
            HelpDialog.DoModal(*this);
        }
        return TRUE;

    // Respond to ToolBar buttons
    case IDM_RED:
        m_penColor = RGB(255, 0, 0);
        TRACE("Red Pen Selected \n");
        return TRUE;
    case IDM_BLUE:
        m_penColor = RGB(0, 0, 255);
        TRACE("Blue Pen Selected \n");
        return TRUE;
    case IDM_GREEN:
        m_penColor = RGB(0, 191, 0);
        TRACE("Green Pen Selected \n");
        return TRUE;
    case IDM_BLACK:
        m_penColor = RGB(0, 0, 0);
        TRACE("Black Pen Selected \n");
        return TRUE;

    // Respond to the accelerator key
    case IDW_QUIT:
        SendMessage(WM_CLOSE, 0L, 0L);
        return TRUE;
    }

    return FALSE;
}

void CMainFrame::OnDraw(CDC& dc)
{
    // Redraw our client area
    if (m_points.size() > 0)
    {
        bool isPenDown = false;  //Start with the pen up
        for (unsigned int i = 0 ; i < m_points.size(); i++)
        {
            dc.CreatePen(PS_SOLID, 1, m_points[i].penColor);
            if (isPenDown)
                dc.LineTo(m_points[i].x, m_points[i].y);
            else
                dc.MoveTo(m_points[i].x, m_points[i].y);

            isPenDown = m_points[i].isPenDown;
        }
    }
}

void CMainFrame::OnInitialUpdate()
{
    // Startup code goes here
}

LRESULT CMainFrame::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);

    // Capture mouse input.
    SetCapture();

    StorePoint(LOWORD(lparam), HIWORD(lparam), true);

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMainFrame::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);

    //Release the capture on the mouse
    ReleaseCapture();

    StorePoint(LOWORD(lparam), HIWORD(lparam), false);

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMainFrame::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);

    // hold down the left mouse button and move mouse to draw lines.
    if (wparam & MK_LBUTTON)
    {
        TCHAR str[80];
        ::wsprintf(str, TEXT("Draw Point:  %hd, %hd\n"), LOWORD(lparam), HIWORD(lparam));
        TRACE(str);

        DrawLine(LOWORD(lparam), HIWORD(lparam));
        StorePoint(LOWORD(lparam), HIWORD(lparam), true);
    }

    return FinalWindowProc(msg, wparam, lparam);
}

void CMainFrame::SetPen(COLORREF color)
{
    m_penColor = color;
}

void CMainFrame::StorePoint(int x, int y, bool isPenDown)
{
    PlotPoint P1;
    P1.x = x;
    P1.y = y;
    P1.isPenDown = isPenDown;
    P1.penColor = m_penColor;

    m_points.push_back(P1); //Add the point to the vector
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // handle left mouse button up/down and mouse move messages
    // a seperate function for each case keeps the code tidy.
    switch (msg)
    {
    case WM_LBUTTONDOWN:    return  OnLButtonDown(msg, wparam, lparam);
    case WM_MOUSEMOVE:      return  OnMouseMove(msg, wparam, lparam);
    case WM_LBUTTONUP:      return  OnLButtonUp(msg, wparam, lparam);
    }

    // Pass unhandled messages on to WndProcDefault
    return WndProcDefault(msg, wparam, lparam);
}


