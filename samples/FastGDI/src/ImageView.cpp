//////////////////////////////////////////////
// ImageView.cpp
//  Definitions for the CImageView class

#include "stdafx.h"
#include "ImageView.h"
#include "resource.h"
#include "UserMessages.h"


constexpr COLORREF black = RGB(0, 0, 0);

///////////////////////////////////
// CImageView function definitions.
//

// Loads a bitmap image from a file.
// Only bitmap images (bmp files) can be loaded.
bool CImageView::LoadFileImage(LPCWSTR fileName)
{
    m_image.Destroy();
    CSize totalSize;

    if (fileName)
    {
        if (m_image.LoadImage(fileName, LR_LOADFROMFILE | LR_CREATEDIBSECTION))
        {
            GetAncestor().SendMessage(UWM_IMAGELOADED, 0, (LPARAM)fileName);

            // Set the image scroll size
            totalSize.cx = m_image.GetSize().cx;
            totalSize.cy = m_image.GetSize().cy;
            SetScrollSizes(totalSize);
        }
        else
        {
            CString str("Failed to load file:  ");
            str += fileName;
            MessageBox(str, L"File Load Error", MB_ICONWARNING);

            // Set Frame title back to default
            GetAncestor().SetWindowText(LoadString(IDW_MAIN).c_str());

            // Disable scrolling
            SetScrollSizes();
            Invalidate();
        }
    }

    return (m_image.GetHandle()!= nullptr);
}

// Called when part of the window needs to be redrawn.
void CImageView::OnDraw(CDC& dc)
{
    if (m_image.GetHandle())
    {
        CMemDC memDC(dc);
        CSize size = m_image.GetSize();
        memDC.SelectObject(m_image);
        dc.BitBlt(0, 0, size.cx, size.cy, memDC, 0, 0, SRCCOPY);
    }
}

// OnInitialUpdate is called after the window is created.
void CImageView::OnInitialUpdate()
{
    TRACE("View window created\n");

    // Set the background color when drawing outside the scrolling area.
    SetScrollBkgnd(CBrush(black));
}

// Select the printer, and call QuickPrint.
void CImageView::Print(LPCWSTR docName)
{
    CPrintDialog printDlg;

    // Bring up a dialog to choose the printer
    if (printDlg.DoModal(*this) == IDOK)    // throws exception if there is no default printer
    {
        QuickPrint(docName);
    }
}

// Prints the image on either the preview pane or the printer.
void CImageView::PrintPage(CDC& dc, int)
{
    try
    {
        if (m_image.GetHandle() != nullptr)
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
            memDC.GetDIBits(m_image, 0, bmHeight, nullptr, pbmi, DIB_RGB_COLORS);
            std::vector<byte> byteArray(pBIH->biSizeImage, 0);
            byte* pByteArray = byteArray.data();
            memDC.GetDIBits(m_image, 0, bmHeight, pByteArray, pbmi, DIB_RGB_COLORS);

            // Copy (stretch) the DI bits to the specified dc.
            dc.StretchDIBits(0, 0, scaledWidth, scaledHeight, 0, 0,
                bmWidth, bmHeight, pByteArray, pbmi, DIB_RGB_COLORS, SRCCOPY);
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Failed", MB_ICONWARNING);
    }
}

// Prints the image on the current default printer.
void CImageView::QuickPrint(LPCWSTR docName)
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
        MessageBox(e.GetText(), L"Print Failed", MB_ICONWARNING);
    }
}

// Saves the bitmap to a file.
bool CImageView::SaveFileImage(LPCWSTR fileName)
{
    CFile file;
    bool result = false;
    try
    {
        file.Open(fileName, OPEN_ALWAYS);

        // Create our LPBITMAPINFO object.
        CBitmapInfoPtr pbmi(m_image);

        // Create the reference DC for GetDIBits to use.
        CMemDC memDC(nullptr);

        // Use GetDIBits to extract the colour data from the CBitmapInfoPtr.
        VERIFY(memDC.GetDIBits(m_image, 0, pbmi->bmiHeader.biHeight, nullptr, pbmi, DIB_RGB_COLORS));
        std::vector<byte> byteArray(pbmi->bmiHeader.biSizeImage, 0);
        byte* pByteArray = byteArray.data();

        VERIFY(memDC.GetDIBits(m_image, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS));

        LPBITMAPINFOHEADER pbmih = &pbmi->bmiHeader;
        BITMAPFILEHEADER hdr{};
        hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"

        // bmiHeader.biClrUsed is cleared by GetDIBits, so we set it again.
        // This is only required for bit counts less than 24.
        if (pbmi->bmiHeader.biBitCount < 24)
            pbmi->bmiHeader.biClrUsed = (1U << pbmi->bmiHeader.biBitCount);

        hdr.bfSize = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof(RGBQUAD) + pbmih->biSizeImage);
        hdr.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + pbmih->biSize + pbmih->biClrUsed * sizeof(RGBQUAD));

        file.Write(&hdr, sizeof(BITMAPFILEHEADER));
        file.Write(pbmih, sizeof(BITMAPINFOHEADER) + pbmih->biClrUsed * sizeof(RGBQUAD));
        file.Write(pByteArray, pbmih->biSizeImage);

        result = true;
    }

    catch (const CFileException& e)
    {
        CString str = CString("Failed to save file: ") + e.GetFilePath();
        MessageBox(str, AtoW(e.what()), MB_OK);
        result = true;
    }

    return result;
}


