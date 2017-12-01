//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "View.h"
#include "RibbonApp.h"
#include "resource.h"


CView::CView() : m_PenColor(RGB(0,0,0))
{
    m_Brush.CreateSolidBrush(RGB(255,255,230));
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
    CMainFrame& frame = GetRibbonFrameApp().GetMainFrame();
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
    memDC.FillRect(GetClientRect(), m_Brush);

    if (GetAllPoints().size() > 0)
    {
        bool bDraw = false;  //Start with the pen up
        for (UINT i = 0 ; i < GetAllPoints().size(); ++i)
        {
            memDC.CreatePen(PS_SOLID, 1, GetAllPoints()[i].color);

            if (bDraw)
                memDC.LineTo(GetAllPoints()[i].x, GetAllPoints()[i].y);
            else
                memDC.MoveTo(GetAllPoints()[i].x, GetAllPoints()[i].y);

            bDraw = GetAllPoints()[i].PenDown;
        }
    }

    // Copy from the memory DC to our painting dc
    dc.BitBlt(0, 0, width, height, memDC, 0, 0, SRCCOPY);
}

LRESULT CView::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(lParam);

    HDROP hDrop = (HDROP)wParam;
    UINT nLength = DragQueryFile(hDrop, 0, 0, 0);

    if (nLength > 0)
    {
        CString FileName;
        DragQueryFile(hDrop, 0, FileName.GetBuffer(nLength), nLength+1);
        FileName.ReleaseBuffer();

        // Send a user defined message to the frame window
        GetParent().SendMessage(UWM_DROPFILE, (WPARAM)FileName.c_str(), 0);

        DragFinish(hDrop);
    }
    return 0L;
}

LRESULT CView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Capture mouse input.
    SetCapture();
    GetDoc().StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true, m_PenColor);

    return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //Release the capture on the mouse
    ReleaseCapture();
    GetDoc().StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false, m_PenColor);

    return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // hold down the left mouse button and move mouse to draw lines.
    if ( (wParam & MK_LBUTTON) && (GetCapture() == *this) )
    {
        CString str;
        str.Format( _T("Draw Point:  %hd, %hd\n"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
        TRACE(str);

        DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        GetDoc().StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true, m_PenColor);
    }
    
    return FinalWindowProc(uMsg, wParam, lParam);
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // Set the extra style to provide a sunken effect
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS& wc)
{
    // Set the background brush, class name and cursor
    wc.hbrBackground = m_Brush;
    wc.lpszClassName = _T("Scribble Window");
    wc.hCursor = GetApp().LoadCursor(IDC_CURSOR1);
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DROPFILES:      return OnDropFiles(uMsg, wParam, lParam);
    case WM_LBUTTONDOWN:    return OnLButtonDown(uMsg, wParam, lParam);
    case WM_MOUSEMOVE:      return OnMouseMove(uMsg, wParam, lParam);
    case WM_LBUTTONUP:      return OnLButtonUp(uMsg, wParam, lParam);   
    }

    //Use the default message handling for remaining messages
    return WndProcDefault(uMsg, wParam, lParam);
}


