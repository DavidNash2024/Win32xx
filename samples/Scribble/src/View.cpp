//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "View.h"
#include "resource.h"


CView::CView() : m_penColor(RGB(0,0,0))
{
    m_brush.CreateSolidBrush(RGB(255,255,230));
}


CView::~CView()
{
}


// Draws a line in the window's client area.
void CView::DrawLine(int x, int y)
{
    CClientDC clientDC(*this);
    clientDC.CreatePen(PS_SOLID, 1, GetAllPoints().back().penColor);
    clientDC.MoveTo(GetAllPoints().back().x, GetAllPoints().back().y);
    clientDC.LineTo(x, y);
}


// Retrieve a reference to CDoc
CDoc& CView::GetDoc()
{
    return m_doc;
}


// Retrieve the PlotPoint data
std::vector<PlotPoint>& CView::GetAllPoints()
{ 
    return GetDoc().GetAllPoints(); 
}


// Called when the window is created.
int CView::OnCreate(CREATESTRUCT&)
{
    // Support Drag and Drop on this window
    DragAcceptFiles(TRUE);
    return 0;
}


// Draws the points to a memory DC. A memory DC provides double buffering for smoother rendering.
CDC CView::Draw()
{
    // Set up our Memory DC and bitmap
    CClientDC dc(*this);
    CMemDC memDC(dc);
    int width = GetClientRect().Width();
    int height = GetClientRect().Height();
    memDC.CreateCompatibleBitmap(dc, width, height);
    memDC.FillRect(GetClientRect(), m_brush);

    // Draw the lines on the memory DC
    if (GetAllPoints().size() > 0)
    {
        bool isDrawing = false;  //Start with the pen up
        for (size_t i = 0 ; i < GetAllPoints().size(); ++i)
        {
            memDC.CreatePen(PS_SOLID, 1, GetAllPoints()[i].penColor);

            if (isDrawing)
                memDC.LineTo(GetAllPoints()[i].x, GetAllPoints()[i].y);
            else
                memDC.MoveTo(GetAllPoints()[i].x, GetAllPoints()[i].y);

            isDrawing = GetAllPoints()[i].isPenDown;
        }
    }

    return memDC;
}


// Called when part of the view window needs to be redawn.
// Calls the Draw function to perform the drawing to a memory DC.
void CView::OnDraw(CDC& dc)
{
    int width = GetClientRect().Width();
    int height = GetClientRect().Height();

    // Copy from the memory DC to our painting dc
    dc.BitBlt(0, 0, width, height, Draw(), 0, 0, SRCCOPY);
}


// Called when a file is dropped on the view window.
LRESULT CView::OnDropFiles(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);

    HDROP hDrop = (HDROP)wparam;
    UINT length = DragQueryFile(hDrop, 0, 0, 0);

    if (length > 0)
    {
        CString FileName;
        DragQueryFile(hDrop, 0, FileName.GetBuffer(length), length +1);
        FileName.ReleaseBuffer();

        // Send a user defined message to the frame window
        GetParent().SendMessage(UWM_DROPFILE, (WPARAM)FileName.c_str(), 0);
    }

    DragFinish(hDrop);
    return 0;
}


// Called when the left mouse buton is pressed.
LRESULT CView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Capture mouse input.
    SetCapture();
    GetDoc().StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), true, m_penColor);
    return FinalWindowProc(msg, wparam, lparam);
}


// Called when the left mouse buton is released.
LRESULT CView::OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    //Release the capture on the mouse
    ReleaseCapture();
    GetDoc().StorePoint(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), false, m_penColor);
    return FinalWindowProc(msg, wparam, lparam);
}


// Called when the mouse is moved while it is captured.
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


// Called before window creation to update the window's CREATESTRUCT
void CView::PreCreate(CREATESTRUCT& cs)
{
    // Set the extra style to provide a sunken effect
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}


// Called before window creation to update the window's CREATESTRUCT
void CView::PreRegisterClass(WNDCLASS& wc)
{
    // Set the background brush, class name and cursor
    wc.hbrBackground = m_brush;
    wc.lpszClassName = _T("Scribble Window");
    wc.hCursor = GetApp()->LoadCursor(IDC_CURSOR1);
}


// Select the printer, and call QuickPrint.
void CView::Print(LPCTSTR docName)
{
    CPrintDialog printDlg;

    // Bring up a dialog to choose the printer
    if (printDlg.DoModal(*this) == IDOK)    // throws exception if there is no default printer
    {
        QuickPrint(docName);
    } 

}


// Prints the view window's bitmap to the specified dc.
// Called by CPrintPreview, and by QuickPrint.
// This function provides a useful reference for printing bitmaps in general.
void CView::PrintPage(CDC& dc, UINT)
{
    // Get the dimensions of the View window
    CRect viewRect = GetClientRect();
    int width = viewRect.Width();
    int height = viewRect.Height();

    // Acquire the view's bitmap.
    CMemDC memDC = Draw();
    CBitmap bmView = Draw().DetachBitmap();

    // Now we convert the Device Dependent Bitmap(DDB) to a Device
    // Independent Bitmap(DIB) for printing or previewing.
    
    // Create the LPBITMAPINFO from the bitmap.
    CBitmapInfoPtr pbmi(bmView);
    // Note: BITMAPINFO and BITMAPINFOHEADER are the same for 24 bit bitmaps
    BITMAPINFOHEADER* pBIH = reinterpret_cast<BITMAPINFOHEADER*>(pbmi.get());

    // Extract the device independent image data.
    memDC.GetDIBits(bmView, 0, height, NULL, pbmi, DIB_RGB_COLORS);
    std::vector<byte> byteArray(pBIH->biSizeImage, 0);
    byte* pByteArray = &byteArray.front();
    memDC.GetDIBits(bmView, 0, height, pByteArray, pbmi, DIB_RGB_COLORS);

    // Get the device context of the default or currently chosen printer
    CPrintDialog printDlg;
    CDC printDC = printDlg.GetPrinterDC();
    
    // Determine the scaling factors required to print the bitmap and retain
    // its original aspect ratio.
    CClientDC viewDC(*this);
    double viewPixelsX = double(viewDC.GetDeviceCaps(LOGPIXELSX));
    double viewPixelsY = double(viewDC.GetDeviceCaps(LOGPIXELSY));
    double printPixelsX = double(printDC.GetDeviceCaps(LOGPIXELSX));
    double printPixelsY = double(printDC.GetDeviceCaps(LOGPIXELSY));
    double scaleX = printPixelsX / viewPixelsX;
    double scaleY = printPixelsY / viewPixelsY;
    int scaledWidth = int(width * scaleX);
    int scaledHeight = int(height * scaleY);

    // Stretch the DIB to the specified dc, maintaining its original aspect ratio.
    UINT result = dc.StretchDIBits(0, 0, scaledWidth, scaledHeight, 0, 0,
        width, height, pByteArray, pbmi, DIB_RGB_COLORS, SRCCOPY);

    if (GDI_ERROR == result)
        throw CUserException(_T("Failed to resize image for printing"));

    // The specified dc now holds the Device Independent Bitmap printout.
}


// Print to the default or previously chosen printer.
void CView::QuickPrint(LPCTSTR docName)
{
    // Create a DOCINFO structure.
    DOCINFO di;
    memset(&di, 0, sizeof(DOCINFO));
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = docName;

    // Get the device context of the default or currently chosen printer.
    CPrintDialog printDlg;
    CDC printDC = printDlg.GetPrinterDC();

    // Begin a print job by calling the StartDoc function.
    if (SP_ERROR == StartDoc(printDC, &di))
        throw CUserException(_T("Failed to start print job"));

    // Inform the driver that the application is about to begin sending data.
    if (0 > StartPage(printDC))
        throw CUserException(_T("StartPage failed"));

    // Print the page on the printer DC
    PrintPage(printDC);

    // Inform the driver that the page is finished.
    if (0 > EndPage(printDC))
        throw CUserException(_T("EndPage failed"));

    // Inform the driver that document has ended.
    if (0 > EndDoc(printDC))
        throw CUserException(_T("EndDoc failed"));
}


// Handle the view's messages.
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


