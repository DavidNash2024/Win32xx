//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "ScribbleApp.h"
#include "resource.h"


CView::CView() : m_penColor(RGB(0,0,0))
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
    clientDC.CreatePen(PS_SOLID, 1, GetAllPoints().back().penColor);
    clientDC.MoveTo(GetAllPoints().back().x, GetAllPoints().back().y);
    clientDC.LineTo(x, y);
}

CDoc& CView::GetDoc()
{
    return m_doc;
}

std::vector<PlotPoint>& CView::GetAllPoints()
{ 
    return GetDoc().GetAllPoints(); 
}

int CView::OnCreate(CREATESTRUCT&)
// Called during window creation
{
    // Support Drag and Drop on this window
    DragAcceptFiles(TRUE);
    return 0;
}

void CView::OnDraw(CDC& dc)
{
    if (GetAllPoints().size() > 0)
    {
        bool isPenDown = false;  //Start with the pen up
        for (size_t i = 0 ; i < GetAllPoints().size(); i++)
        {
            dc.CreatePen(PS_SOLID, 1, GetAllPoints()[i].penColor);
            if (isPenDown)
                dc.LineTo(GetAllPoints()[i].x, GetAllPoints()[i].y);
            else
                dc.MoveTo(GetAllPoints()[i].x, GetAllPoints()[i].y);
            
            isPenDown = GetAllPoints()[i].isPenDown;
        }
    }
}

LRESULT CView::OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam)
// Called when a file is dropped on the window
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);

    HDROP hDrop = (HDROP)wparam;
    UINT length = DragQueryFile(hDrop, 0, 0, 0);

    if (length > 0)
    {
        CString fileName;
        DragQueryFile(hDrop, 0, fileName.GetBuffer(length), length +1);
        fileName.ReleaseBuffer();

        // Send a user defined message to the frame window
        GetParent().SendMessage(UWM_DROPFILE, (WPARAM)fileName.c_str(), 0);

        DragFinish(hDrop);
    }
    return 0;
}

LRESULT CView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
// Called when the left mouse button is pressed while the cursor is over the window.
{
    // Capture mouse input.
    SetCapture();
    GetDoc().StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), true, m_penColor);

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
// Called when the left mouse button is released
{
    //Release the capture on the mouse
    ReleaseCapture();
    GetDoc().StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), false, m_penColor);

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
// Called when the mouse is moved while captured
{
    // hold down the left mouse button and move mouse to draw lines.
    if ( (wparam & MK_LBUTTON) && (GetCapture() == *this) )
    {
        DrawLine(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
        GetDoc().StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), true, m_penColor);
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
    wc.hCursor = GetApp()->LoadCursor(IDC_CURSOR1);
}

// Sends the bitmap extracted from the View window to a printer of your choice.
// This function provides a useful reference for printing bitmaps in general.
// This function throws an exception if unable to print.
void CView::Print()
{
    // Get the dimensions of the View window
    CRect viewRect = GetClientRect();
    int width = viewRect.Width();
    int height = viewRect.Height();

    // Copy the bitmap from the View window
    CClientDC viewDC(*this);
    CMemDC memDC(viewDC);
    memDC.CreateCompatibleBitmap(viewDC, width, height);
    BitBlt(memDC, 0, 0, width, height, viewDC, 0, 0, SRCCOPY);
    CBitmap bmView = memDC.DetachBitmap();
    CPrintDialog printDlg;

    // Bring up a dialog to choose the printer
    if (printDlg.DoModal(*this) == IDOK)    // throws exception if there is no default printer
    {
        // Zero and then initialize the members of a DOCINFO structure.
        DOCINFO di;
        memset(&di, 0, sizeof(DOCINFO));
        di.cbSize = sizeof(DOCINFO);
        di.lpszDocName = _T("Scribble Printout");

        // Begin a print job by calling the StartDoc function.
        CDC printDC = printDlg.GetPrinterDC();
        if (SP_ERROR == StartDoc(printDC, &di))
            throw CUserException(_T("Failed to start print job"));

        // Inform the driver that the application is about to begin sending data.
        if (0 > StartPage(printDC))
            throw CUserException(_T("StartPage failed"));

        BITMAPINFOHEADER bih;
        ZeroMemory(&bih, sizeof(bih));
        bih.biSize = sizeof(bih);
        bih.biHeight = height;
        bih.biWidth = width;
        bih.biPlanes = 1;
        bih.biBitCount = 24;
        bih.biCompression = BI_RGB;

        // Note: BITMAPINFO and BITMAPINFOHEADER are the same for 24 bit bitmaps
        // Get the size of the image data
        BITMAPINFO* pBI = reinterpret_cast<BITMAPINFO*>(&bih);
        memDC.GetDIBits(bmView, 0, height, NULL, pBI, DIB_RGB_COLORS);

        // Retrieve the image data
        std::vector<byte> vBits(bih.biSizeImage, 0); // a vector to hold the byte array
        byte* pByteArray = &vBits.front();
        memDC.GetDIBits(bmView, 0, height, pByteArray, pBI, DIB_RGB_COLORS);

        // Determine the scaling factors required to print the bitmap and retain its original proportions.
        double viewPixelsX  = double(viewDC.GetDeviceCaps(LOGPIXELSX));
        double viewPixelsY  = double(viewDC.GetDeviceCaps(LOGPIXELSY));
        double printPixelsX = double(printDC.GetDeviceCaps(LOGPIXELSX));
        double printPixelsY = double(printDC.GetDeviceCaps(LOGPIXELSY));
        double scaleX = printPixelsX / viewPixelsX;
        double scaleY = printPixelsY / viewPixelsY;

        int scaledWidth = int(width * scaleX);
        int scaledHeight = int(height * scaleY);

        // Use StretchDIBits to scale the bitmap and maintain its original proportions
        UINT result = StretchDIBits(printDC, 0, 0, scaledWidth, scaledHeight, 0, 0,
            width, height, pByteArray, pBI, DIB_RGB_COLORS, SRCCOPY);

        if (GDI_ERROR == result)
            throw CUserException(_T("Failed to resize image for printing"));

        // Inform the driver that the page is finished.
        if (0 > EndPage(printDC))
            throw CUserException(_T("EndPage failed"));

        // Inform the driver that document has ended.
        if (0 > EndDoc(printDC))
            throw CUserException(_T("EndDoc failed"));
    }
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
// Called to handle the window's messages
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

