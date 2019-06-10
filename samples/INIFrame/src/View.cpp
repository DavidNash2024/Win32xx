//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"


CView::CView()
{
}


// Returns a reference to CDoc.
CDoc& CView::GetDoc()
{
    return m_doc;
}


// OnDraw is called when part or all of the window needs to be redrawn.
void CView::OnDraw(CDC& dc)
{
    CRect rc = GetClientRect();

    // Centre some text in our view window
    dc.DrawText(_T("View Window"), -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


// OnInitialUpdate is called immediately after the window is created.
void CView::OnInitialUpdate()
{
    TRACE("View window created\n");
}


// Called before window creation to update the window's CREATESTRUCT.
void CView::PreCreate(CREATESTRUCT& cs)
{
    // Here we set the defaults used by the create function for the view window
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the window we create.

    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}


// Called before the window is registered to update the window's WNDCLASS.
void CView::PreRegisterClass(WNDCLASS& wc)
{
    // Here we set the Window class parameters.
    // Preforming this is optional, but doing so allows us to
    // take more precise control over the type of window we create.

    // Set the Window Class name
    wc.lpszClassName = _T("Win32++ View");

    // Set a background brush to white
    wc.hbrBackground = reinterpret_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));

    // Set the default cursor
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);

    // Set the class style (not to be confused with the window styles set in PreCreate)
    wc.style = CS_DBLCLKS;  // Generate left button double click messages
}


void CView::Print(LPCTSTR docName)
{
    SaveViewImage();
    QuickPrint(docName);
}

// Prints the specified page to the specified device context.
// Used when printing and previewing.
void CView::PrintPage(CDC& dc, UINT)
{
    // Here we copy (stretch) a bitmap image of the view window
    // to the printed page.

    // Copy the view image to a memory DC.
    CClientDC viewDC(*this);
    CMemDC memDC(dc);
    int cxImage = m_viewImage.GetBitmapData().bmWidth;
    int cyImage = m_viewImage.GetBitmapData().bmHeight;
    memDC.CreateCompatibleBitmap(dc, cxImage, cyImage);
    memDC.SelectObject(m_viewImage);

    // Get the device context of the default or currently chosen printer
    CPrintDialog printDlg;
    CDC printDC = printDlg.GetPrinterDC();

    int cxPage = printDC.GetDeviceCaps(HORZRES);
    int cyPage = printDC.GetDeviceCaps(VERTRES);

    dc.StretchBlt(0, 0, cxPage, cyPage, memDC, 0, 0, cxImage, cyImage, SRCCOPY);
}

// Print to the default or previously chosen printer.
void CView::QuickPrint(LPCTSTR docName)
{
    try
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

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }
}


// Save a bitmap image of the view window for printing.
void CView::SaveViewImage()
{
    CClientDC viewDC(*this);
    CMemDC memDC(viewDC);
    int cxView = GetClientRect().Width();
    int cyView = GetClientRect().Height();
    memDC.CreateCompatibleBitmap(viewDC, cxView, cyView);
    memDC.BitBlt(0, 0, cxView, cyView, viewDC, 0, 0, SRCCOPY);
    m_viewImage = memDC.DetachBitmap();
}


// All window messages for this window pass through WndProc.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_SIZE:
        Invalidate();
        break;  // Also do default processing
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}



