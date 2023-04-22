//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "View.h"
#include "FastGDIApp.h"
#include "resource.h"

/////////////////////////////
// CView function definitions
//

// Constructor.
CView::CView()
{
}

// Destructor.
CView::~CView()
{
}

// Loads a bitmap image from a file.
// Only bitmap images (bmp files) can be loaded.
BOOL CView::LoadFileImage(LPCTSTR filename)
{
    m_image.DeleteObject();
    CSize totalSize;

    if (filename)
    {
        if (!m_image.LoadImage(filename, LR_LOADFROMFILE | LR_CREATEDIBSECTION))
        {
            CString str("Failed to load file:  ");
            str += filename;
            MessageBox(str, _T("File Load Error"), MB_ICONWARNING);
        }
    }

    if (m_image.GetHandle())
    {
        // Set the image scroll size
        totalSize.cx = GetImageRect().Width();
        totalSize.cy = GetImageRect().Height();
    }
    else
    {
        // Disable scrolling
        totalSize = CSize(0, 0);
        Invalidate();
    }

    SetScrollSizes(totalSize);
    return (m_image.GetHandle()!= 0);
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

// Prints the image on either the preview pane or the printer.
void CView::PrintPage(CDC& dc, UINT)
{
    try
    {
        if (m_image.GetHandle() != 0)
        {
            BITMAP bitmap = m_image.GetBitmapData();
            int bmWidth = bitmap.bmWidth;
            int bmHeight = bitmap.bmHeight;

            // Get the device context of the default or currently chosen printer
            CPrintDialog printDlg;
            CDC printDC = printDlg.GetPrinterDC();
            CClientDC viewDC(*this);
            double viewPixelsX = double(viewDC.GetDeviceCaps(LOGPIXELSX));
            double viewPixelsY = double(viewDC.GetDeviceCaps(LOGPIXELSY));
            double printPixelsX = double(printDC.GetDeviceCaps(LOGPIXELSX));
            double printPixelsY = double(printDC.GetDeviceCaps(LOGPIXELSY));
            double scaleX = printPixelsX / viewPixelsX;
            double scaleY = printPixelsY / viewPixelsY;
            int scaledWidth = int(bmWidth * scaleX);
            int scaledHeight = int(bmHeight * scaleY);

            // Create the LPBITMAPINFO from the bitmap.
            CBitmapInfoPtr pbmi(m_image);
            BITMAPINFOHEADER* pBIH = reinterpret_cast<BITMAPINFOHEADER*>(pbmi.get());

            // Extract the device independent image data.
            CMemDC memDC(viewDC);
            memDC.GetDIBits(m_image, 0, bmHeight, NULL, pbmi, DIB_RGB_COLORS);
            std::vector<byte> byteArray(pBIH->biSizeImage, 0);
            byte* pByteArray = &byteArray.front();
            memDC.GetDIBits(m_image, 0, bmHeight, pByteArray, pbmi, DIB_RGB_COLORS);

            // Copy (stretch) the DI bits to the specified dc.
            dc.StretchDIBits(0, 0, scaledWidth, scaledHeight, 0, 0,
                bmWidth, bmHeight, pByteArray, pbmi, DIB_RGB_COLORS, SRCCOPY);
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Failed"), MB_ICONWARNING);
    }
}

// Prints the image on the current default printer.
void CView::QuickPrint(LPCTSTR docName)
{
    try
    {
        // Create a DOCINFO structure.
        DOCINFO di;
        memset(&di, 0, sizeof(DOCINFO));
        di.cbSize = sizeof(DOCINFO);
        di.lpszDocName = docName;

        CPrintDialog printDlg;
        CDC printDC = printDlg.GetPrinterDC();

        printDC.StartDoc(&di);
        printDC.StartPage();

        PrintPage(printDC);

        printDC.EndPage();
        printDC.EndDoc();
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Failed"), MB_ICONWARNING);
    }
}

// Saves the bitmap to a file.
BOOL CView::SaveFileImage(LPCTSTR fileName)
{
    CFile file;
    BOOL result = FALSE;
    try
    {
        file.Open(fileName, OPEN_ALWAYS);

       // Create our LPBITMAPINFO object
       CBitmapInfoPtr pbmi(m_image);

       // Create the reference DC for GetDIBits to use
       CMemDC memDC(0);

       // Use GetDIBits to create a DIB from our DDB, and extract the colour data
       VERIFY(memDC.GetDIBits(m_image, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS));
       std::vector<byte> byteArray(pbmi->bmiHeader.biSizeImage, 0);
       byte* pByteArray = &byteArray.front();

       VERIFY(memDC.GetDIBits(m_image, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS));

       LPBITMAPINFOHEADER pbmih = &pbmi->bmiHeader;
       BITMAPFILEHEADER hdr;
       ZeroMemory(&hdr, sizeof(BITMAPFILEHEADER));
       hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
       hdr.bfSize = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof(RGBQUAD) + pbmih->biSizeImage);
       hdr.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof (RGBQUAD));

       file.Write(&hdr, sizeof(BITMAPFILEHEADER));
       file.Write(pbmih, sizeof(BITMAPINFOHEADER) + pbmih->biClrUsed * sizeof (RGBQUAD));
       file.Write(pByteArray, pbmih->biSizeImage);

       result = TRUE;
    }

    catch (const CFileException& e)
    {
        CString str = CString("Failed to save file: ") + e.GetFilePath();
        MessageBox(str, AtoT(e.what()), MB_OK);
        result = FALSE;
    }

    return result;
}

// Retrieves the image size as a rectangle.
CRect CView::GetImageRect()
{
    BITMAP bm = m_image.GetBitmapData();

    CRect rc;
    rc.right = bm.bmWidth;
    rc.bottom = bm.bmHeight;

    return rc;
}

// Called when part of the window needs to be redrawn.
void CView::OnDraw(CDC& dc)
{
    if (m_image.GetHandle())
    {
        CMemDC memDC(dc);
        CSize size = m_image.GetSize();
        memDC.CreateCompatibleBitmap(dc, size.cx, size.cy);
        memDC.SelectObject(m_image);
        dc.BitBlt(0, 0, size.cx, size.cy, memDC, 0, 0, SRCCOPY);
    }
    else
    {
        // There is no image, so display a hint to get one

            // Use the message font for Windows 7 and higher.
        if (GetWinVersion() >= 2601)
        {
            NONCLIENTMETRICS info = GetNonClientMetrics();
            LOGFONT lf = DPIScaleLogfont(info.lfMessageFont, 10);
            dc.CreateFontIndirect(lf);
        }

        CRect rc = GetClientRect();
        dc.DrawText(_T("Use the Menu or ToolBar to open a Bitmap File"), -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

// Called when a file is dropped on the window.
// Loads the dropped file.
LRESULT CView::OnDropFiles(UINT, WPARAM wparam, LPARAM)
{
    HDROP hDrop = (HDROP)wparam;
    UINT length = DragQueryFile(hDrop, 0, 0, 0);

    if (length > 0)
    {
        CString fileName;
        DragQueryFile(hDrop, 0, fileName.GetBuffer(length), length+1);
        fileName.ReleaseBuffer();
        DragFinish(hDrop);

        CMainFrame& Frame = GetFrameApp()->GetMainFrame();

        if ( !Frame.LoadFile(fileName) )
        {
            TRACE ("Failed to load "); TRACE(fileName); TRACE("\n");
            SetScrollSizes(CSize(0,0));
            Invalidate();
        }
    }

    return 0;
}

// OnInitialUpdate is called after the window is created.
void CView::OnInitialUpdate()
{
    TRACE("View window created\n");

    // Support Drag and Drop on this window
    DragAcceptFiles(TRUE);
}

// Sets the CREATESTRUCT parameters before the window is created.
void CView::PreCreate(CREATESTRUCT& cs)
{
    // Set the Window Class name
    cs.lpszClass = _T("View");

    cs.style = WS_CHILD | WS_HSCROLL | WS_VSCROLL ;

    // Set the extended style
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

// Process the view's window messages.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DROPFILES:          return OnDropFiles(msg, wparam, lparam);
        }

        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

