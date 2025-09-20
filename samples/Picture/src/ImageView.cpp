/////////////////////////////
// ImageView.cpp
//

#include "stdafx.h"
#include "ImageView.h"
#include "UserMessages.h"
#include "resource.h"

#define HIMETRIC_INCH   2540

constexpr COLORREF black = RGB(0, 0, 0);

///////////////////////////////////
// CImageView function definitions.
//

// Constructor.
CImageView::CImageView() : m_pPicture(nullptr)
{
    // Initializes the COM library on the current thread.
    if FAILED(::CoInitialize(nullptr))
        throw CWinException(L"Failed to initialize COM");
}

// Destructor.
CImageView::~CImageView()
{
    if (m_pPicture)
        m_pPicture->Release();

    ::CoUninitialize();
}

// Retrieves the width and height of picture.
CSize CImageView::GetImageSize() const
{
    long width = 0;
    long height = 0;

    if (m_pPicture)
    {
        m_pPicture->get_Width(&width);
        m_pPicture->get_Height(&height);
    }

    // convert himetric to pixels
    CClientDC dc(*this);
    int widthInPixels  = MulDiv(width, dc.GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
    int heightInPixels = MulDiv(height, dc.GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);

    CSize imageSize;
    imageSize.cx = std::max(widthInPixels, 200);
    imageSize.cy = std::max(heightInPixels, 200);
    return imageSize;
}

// Displays a blank (black) image.
void CImageView::NewPictureFile()
{
    if (m_pPicture)
    {
        m_pPicture->Release();
        m_pPicture = nullptr;

        // Turn scrolling off
        SetScrollSizes();
    }

    GetAncestor().SetWindowText(LoadString(IDW_MAIN).c_str());
    Invalidate();
}

// Loads an image from the specified file.
bool CImageView::LoadPictureFile(LPCWSTR fileName)
{
    if (m_pPicture)
    {
        m_pPicture->Release();
        m_pPicture = nullptr;
    }

    bool isPictureLoaded;

    // Create IPicture from image file
    if (S_OK == ::OleLoadPicturePath(WtoOLE(fileName), nullptr, 0, 0, IID_IPicture, (LPVOID*)&m_pPicture))
    {
        GetAncestor().SendMessage(UWM_IMAGELOADED, 0, (LPARAM)fileName);
        Invalidate();
        CSize size = GetImageSize();
        SetScrollSizes(size);
        isPictureLoaded = true;

        TRACE("Successfully loaded: "); TRACE(fileName); TRACE("\n");
    }
    else
    {
        CString str("Failed to load: ");
        str += fileName;
        MessageBox(str, L"Load Picture Failed", MB_ICONWARNING);
        TRACE(str); TRACE("\n");

        // Set Frame title back to default
        GetAncestor().SetWindowText(LoadString(IDW_MAIN).c_str());
        SetScrollSizes();
        isPictureLoaded = false;
    }

    return isPictureLoaded;
}

// Called when the window is created.
int CImageView::OnCreate(CREATESTRUCT& cs)
{
    // Set the window background to black
    m_brush.CreateSolidBrush(black);
    SetClassLongPtr(GCLP_HBRBACKGROUND, (LONG_PTR)m_brush.GetHandle());

    // Set a black background brush for scrolling.
    SetScrollBkgnd(m_brush);

    return CWnd::OnCreate(cs);
}

// Called when part of the window needs to be redrawn.
void CImageView::OnDraw(CDC& dc)
{
    if (m_pPicture)
    {
        // get width and height of picture
        long width;
        long height;
        m_pPicture->get_Width(&width);
        m_pPicture->get_Height(&height);

        // convert himetric to pixels
        int widthInPixels = MulDiv(width, GetDeviceCaps(dc, LOGPIXELSX), HIMETRIC_INCH);
        int HeightInPixels = MulDiv(height, GetDeviceCaps(dc, LOGPIXELSY), HIMETRIC_INCH);

        // Render the picture to the DC
        CRect rc;
        m_pPicture->Render(dc, 0, 0, widthInPixels, HeightInPixels, 0, height, width, -height, &rc);
    }
}

// Saves the image to the specified file.
void CImageView::SavePicture(LPCWSTR fileName)
{
    // get a IPictureDisp interface from your IPicture pointer
    IPictureDisp *pDisp = nullptr;

    if (SUCCEEDED(m_pPicture->QueryInterface(IID_IPictureDisp,  (void**) &pDisp)))
    {
        // Save the IPicture image as a bitmap
        OleSavePictureFile(pDisp,  WtoBSTR(fileName));
        pDisp->Release();
    }
}

