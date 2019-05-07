
#include "stdafx.h"
#include "Doc.h"
#include "ScribbleApp.h"


const CView& CDoc::GetView() const
{
    CMainFrame& frame = GetScribbleApp()->GetMainFrame();
    return static_cast<CView&>(frame.GetView());
}

BOOL CDoc::FileOpen(LPCTSTR filename)
{
    GetAllPoints().clear();
    BOOL isFileOpened = FALSE;

    try
    {
        CArchive ar(filename, CArchive::load);
        ar >> *this;
        isFileOpened = TRUE;
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);
        
        GetAllPoints().clear();
    }

    return isFileOpened;
}

BOOL CDoc::FileSave(LPCTSTR filename)
{
    BOOL isFileSaved = FALSE;

    try
    {
        CArchive ar(filename, CArchive::store);
        ar << *this;
        isFileSaved = TRUE;
    }
    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Save File"), MB_ICONWARNING);
    }

    return isFileSaved;
}

// Sends the bitmap extracted from the View window to a printer of your choice
// This function provides a useful reference for printing bitmaps in general
void CDoc::Print()
{
    // Get the dimensions of the View window
    CRect viewRect = GetView().GetClientRect();
    int width = viewRect.Width();
    int height = viewRect.Height();

    // Copy the bitmap from the View window
    CClientDC viewDC(GetView());
    CMemDC memDC(viewDC);
    memDC.CreateCompatibleBitmap(viewDC, width, height);
    BitBlt(memDC, 0, 0, width, height, viewDC, 0, 0, SRCCOPY);
    CBitmap bmView = memDC.DetachBitmap();
    CPrintDialog printDlg;

    try
    {
        // Bring up a dialog to choose the printer
        if (printDlg.DoModal(GetView()) == IDOK)    // throws exception if there is no default printer
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
            double viewPixelsX = double(viewDC.GetDeviceCaps(LOGPIXELSX));
            double viewPixelsY = double(viewDC.GetDeviceCaps(LOGPIXELSY));
            double printPixelsX = double(GetDeviceCaps(printDC, LOGPIXELSX));
            double printPixelsY = double(GetDeviceCaps(printDC, LOGPIXELSY));
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

    catch (const CException& e)
    {
        // Display a message box indicating why printing failed.
        CString message = CString(e.GetText()) + CString("\n") + e.GetErrorString();
        CString type = CString(e.what());
        ::MessageBox(NULL, message, type, MB_ICONWARNING);
    }
}

void CDoc::Serialize(CArchive &ar)
// Uses CArchive to stream data to or from a file
{

    if (ar.IsStoring())
    {
        // Store the number of points
        UINT points = UINT(GetAllPoints().size());
        ar << points;
        
        // Store the PlotPoint data
        std::vector<PlotPoint>::iterator iter;
        for (iter = GetAllPoints().begin(); iter < GetAllPoints().end(); ++iter)
        {
            ArchiveObject ao( &(*iter), sizeof(PlotPoint) );
            ar << ao;
        }
    }
    else
    {
        UINT points;
        PlotPoint pp;
        GetAllPoints().clear();

        // Load the number of points
        ar >> points;

        // Load the PlotPoint data
        for (UINT u = 0; u < points; ++u)
        {
            ArchiveObject ao( &pp, sizeof(pp) );
            ar >> ao;
            GetAllPoints().push_back(pp);
        }
    }

}

void CDoc::StorePoint(int x, int y, bool isPenDown, COLORREF penColor)
{
    PlotPoint pp;
    pp.x = x;
    pp.y = y;
    pp.isPenDown = isPenDown;
    pp.penColor = penColor;

    m_points.push_back(pp); //Add the point to the vector
}