//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "View.h"
#include "RibbonApp.h"
#include "resource.h"


CView::CView() : m_penColor(RGB(0,0,0))
{
    m_brush.CreateSolidBrush(RGB(255,255,230));
}

CView::~CView()
{
}

void CView::DrawLine(int x, int y)
{
    CClientDC clientDC(*this);
    clientDC.CreatePen(PS_SOLID, 1, GetAllPoints().back().color);
    clientDC.MoveTo(GetAllPoints().back().x, GetAllPoints().back().y);
    clientDC.LineTo(x, y);
}

CDoc& CView::GetDoc()
{
    CMainFrame& frame = GetRibbonFrameApp()->GetMainFrame();
    return frame.GetDoc();
}

std::vector<PlotPoint>& CView::GetAllPoints()
{ 
    return GetDoc().GetAllPoints(); 
}

int CView::OnCreate(CREATESTRUCT&)
{
    // Support Drag and Drop on this window
    DragAcceptFiles(TRUE);
    return 0;
}

void CView::OnDraw(CDC& dc)
{
    // Here we use double buffering (drawing to a memory DC) for smoother rendering
    // Set up our Memory DC and bitmap
    CMemDC memDC(dc);
    int width = GetClientRect().Width();
    int height = GetClientRect().Height();
    memDC.CreateCompatibleBitmap(dc, width, height);
    memDC.FillRect(GetClientRect(), m_brush);

    if (GetAllPoints().size() > 0)
    {
        bool isPenDown = false;  //Start with the pen up
        for (size_t i = 0 ; i < GetAllPoints().size(); ++i)
        {
            memDC.CreatePen(PS_SOLID, 1, GetAllPoints()[i].color);

            if (isPenDown)
                memDC.LineTo(GetAllPoints()[i].x, GetAllPoints()[i].y);
            else
                memDC.MoveTo(GetAllPoints()[i].x, GetAllPoints()[i].y);

			isPenDown = GetAllPoints()[i].isPenDown;
        }
    }

    // Copy from the memory DC to our painting dc
    dc.BitBlt(0, 0, width, height, memDC, 0, 0, SRCCOPY);
}

LRESULT CView::OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);

    HDROP hDrop = (HDROP)wparam;
    UINT length = DragQueryFile(hDrop, 0, 0, 0);

    if (length > 0)
    {
        CString fileName;
        DragQueryFile(hDrop, 0, fileName.GetBuffer(length), length+1);
        fileName.ReleaseBuffer();

        // Send a user defined message to the frame window
        GetParent().SendMessage(UWM_DROPFILE, (WPARAM)fileName.c_str(), 0);
    }

    DragFinish(hDrop);
    return 0;
}

LRESULT CView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Capture mouse input.
    SetCapture();
    GetDoc().StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), true, m_penColor);

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    //Release the capture on the mouse
    ReleaseCapture();
    GetDoc().StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), false, m_penColor);

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // hold down the left mouse button and move mouse to draw lines.
    if ( (wparam & MK_LBUTTON) && (GetCapture() == *this) )
    {
        CString str;
        str.Format( _T("Draw Point:  %hd, %hd\n"), GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
        TRACE(str);

        DrawLine(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
        GetDoc().StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), true, m_penColor);
    }
    
    return FinalWindowProc(msg, wparam, lparam);
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // Set the extra style to provide a sunken effect
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS& wc)
{
    // Set the background brush, class name and cursor
    wc.hbrBackground = m_brush;
    wc.lpszClassName = _T("Scribble Window");
    wc.hCursor = GetApp()->LoadCursor(IDC_CURSOR1);
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DROPFILES:      return OnDropFiles(msg, wparam, lparam);
    case WM_LBUTTONDOWN:    return OnLButtonDown(msg, wparam, lparam);
    case WM_MOUSEMOVE:      return OnMouseMove(msg, wparam, lparam);
    case WM_LBUTTONUP:      return OnLButtonUp(msg, wparam, lparam);
    }

    //Use the default message handling for remaining messages
    return WndProcDefault(msg, wparam, lparam);
}


