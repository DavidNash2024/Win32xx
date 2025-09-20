/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"
#include "resource.h"

#if defined (_MSC_VER) && (_MSC_VER == 1900) // == VS2015
#pragma warning (disable : 4458) // disable warning: declaration hides class member.

// Declare min and max for Visual Studio 2015.
using std::min;
using std::max;
#endif

#include <gdiplus.h>

#if defined (_MSC_VER) && (_MSC_VER == 1900) // == VS2015
#pragma warning (default : 4458) // return warning to default.
#endif

using namespace Gdiplus;


/////////////////////////////
// CView function definitions
//

CView::CView() : m_mask(RGB(192,192,192)), m_colorBits(24), m_isMasked(false)
{
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
}

CView::~CView()
{
    // Shutdown GDI+
    GdiplusShutdown(m_gdiplusToken);
}

// Create the image list and store the colorBits information.
void CView::CreateImageList(int imageWidth, int imageHeight, int colorBits)
{
    m_toolbarImages.Create(imageWidth, imageHeight, colorBits, 1, 1);
    m_colorBits = colorBits;
}

// Builds a Device Independent Bitmap(DIB) from the images in the image list.
CBitmap CView::GetBitmapFromImageList() const
{
    // Retrieves the IMAGEINFO struct from the image list.
    // The IMAGEINFO's hbmImage member holds the vertical bitmap.
    IMAGEINFO info = {};
    m_toolbarImages.GetImageInfo(0, &info);
    CMemDC dcHorizontal(nullptr);

    if (info.hbmImage != nullptr)
    {
        // Create the buffer to hold the data for the vertical DIB.
        CBitmapInfoPtr pbmi(info.hbmImage);
        CMemDC dcVertical(nullptr);
        VERIFY(dcVertical.GetDIBits(info.hbmImage, 0, pbmi->bmiHeader.biHeight,
            nullptr, pbmi, DIB_RGB_COLORS));
        std::vector<byte> byteArray(pbmi->bmiHeader.biSizeImage, 0);
        void* pBuffer = byteArray.data();

        // Create and select the vertical DIB created from info.hbmImage.
        dcVertical.CreateDIBSection(dcVertical, pbmi, DIB_RGB_COLORS, &pBuffer,
            nullptr, 0);
        VERIFY(dcVertical.GetDIBits(info.hbmImage, 0, pbmi->bmiHeader.biHeight,
            pBuffer, pbmi, DIB_RGB_COLORS));

        // Create the horizontal DIB from the vertical DIB.
        int imageCount = m_toolbarImages.GetImageCount();
        int cx = m_toolbarImages.GetIconSize().cx;
        int cy = m_toolbarImages.GetIconSize().cy;
        BITMAP bm = {};
        bm.bmWidth = cx * imageCount;
        bm.bmHeight = cy;
        bm.bmBitsPixel = pbmi->bmiHeader.biBitCount;
        CBitmapInfoPtr pbmiHorizontal(bm);

        dcHorizontal.CreateDIBSection(dcHorizontal, pbmiHorizontal, DIB_RGB_COLORS,
            nullptr, nullptr, 0);

        // Copy the color table from the vertical to the horizontal DIB.
        if (bm.bmBitsPixel <= 8)
        {
            int numColors = 1 << bm.bmBitsPixel;
            std::vector<RGBQUAD> colors(numColors);
            RGBQUAD* colorTable = colors.data();
            ::GetDIBColorTable(dcVertical, 0, numColors, colorTable);
            ::SetDIBColorTable(dcHorizontal, 0, numColors, colorTable);
        }

        // Copy each image from the vertical to the horizontal DIB.
        for (int i = 0; i < imageCount; i++)
        {
            dcHorizontal.BitBlt(i * cx, 0, cx, cy, dcVertical, 0, i * cy, SRCCOPY);
        }
    }

    // Return the horizontal DIB.
    return dcHorizontal.DetachBitmap();
}

// Retrieves the number of pressed toolbar buttons.
int CView::GetPressedButtons() const
{
    int buttonCount = m_toolbar.GetButtonCount();
    int pressedButtons = 0;
    for (int i = 0; i < buttonCount; i++)
    {
        if (m_toolbar.GetButtonState(i + TB_BUTTON0) & TBSTATE_CHECKED)
            pressedButtons++;
    }

    return pressedButtons;
}

// Adds an image to the image list.
bool CView::ImageAdd(const CString& pathName)
{
    // Attempt to load the file as an icon.
    // Loads the icon that best matches the image size in the image list.
    int imageCount = m_toolbarImages.GetImageCount();
    CSize sz = m_toolbarImages.GetIconSize();
    HICON icon = (HICON)GetApp()->LoadImage(pathName, IMAGE_ICON,
        sz.cx, sz.cy, LR_LOADFROMFILE);
    if (icon != nullptr)
        m_toolbarImages.Add(icon);
    else
    {
        // Attempt to load the file as a device independent bitmap.
        HBITMAP hbm = (HBITMAP)GetApp()->LoadImage(pathName, IMAGE_BITMAP,
            0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hbm == nullptr)
        {
            // Use GDI+ to load the image when LoadImage fails. The graphics
            // file formats supported by GDI+ are:
            // BMP; GIF; JPEG; PNG; TIFF; Exif; WMF; and EMF.
            Gdiplus::Bitmap bm(pathName);
            if (bm.GetWidth() > 0)
            {
                Color clr{};
                bm.GetHBITMAP(clr, &hbm);
            }
        }

        if (hbm != nullptr)
        {
            // Resize the bitmap to fit in the image list.
            CBitmap bitmap(hbm);
            CSize imageSize = bitmap.GetSize();
            double size = std::max((double)sz.cy, (double)16);
            double scale = size / std::max((double)imageSize.cy, (double)16);
            CBitmap resized = ResizeBitmap(bitmap, scale, scale);

            // Add the resized bitmap to the image list.
            m_toolbarImages.Add(resized);
        }
    }

    UpdateToolbar();
    RecalcLayout();
    return m_toolbarImages.GetImageCount() > imageCount;
}

// Deletes the selected image(s).
bool CView::ImageDelete()
{
    int buttonCount = m_toolbar.GetButtonCount();
    for (int i = buttonCount - 1; i >= 0; i--)
    {
        if (m_toolbar.GetButtonState(i + TB_BUTTON0) & TBSTATE_CHECKED)
        {
            m_toolbarImages.Remove(i);
        }
    }

    m_pager.RecalcSize();

    UpdateToolbar();
    return true;
}

// Swaps the two selected images.
bool CView::ImageSwap()
{
    int buttonCount = m_toolbar.GetButtonCount();
    int image1 = -1;
    int image2 = -1;
    for (int i = 0; i < buttonCount; i++)
    {
        if (m_toolbar.GetButtonState(i + TB_BUTTON0) & TBSTATE_CHECKED)
        {
            if (image1 == -1)
                image1 = i;
            else
            {
                image2 = i;
                break;
            }
        }
    }

    bool result = m_toolbarImages.Copy(image1, image2, ILCF_SWAP) != 0;
    SetToolbarImageList();
    Invalidate();
    return result;
}

// Loads the toolbar bitmap from the specified file.
bool CView::LoadToolbar(const CString& fileName)
{
    CBitmap toolbarImage;
    toolbarImage.LoadImage(fileName, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    if (toolbarImage.GetHandle() != nullptr)
    {
        BITMAP bm = toolbarImage.GetBitmapData();
        int imageHeight = bm.bmHeight;
        int imageWidth = std::max(imageHeight, 16);  // Also support size 16x15.

        WORD colorBits = bm.bmBitsPixel;
        if (colorBits <= 1)       colorBits = 0;
        else if (colorBits <= 4)  colorBits = 4;
        else if (colorBits <= 8)  colorBits = 8;
        else if (colorBits <= 16) colorBits = 16;
        else if (colorBits <= 24) colorBits = 24;
        else                      colorBits = 32;

        CreateImageList(imageWidth, imageHeight, colorBits);
        m_toolbarImages.Add(toolbarImage);
        UpdateToolbar();
        Invalidate();
        return true;
    }

    return false;
}

// Creates a new image list to hold the toolbar images.
bool CView::NewToolbar(int imageSize, int colorBits)
{
    CreateImageList(imageSize, imageSize, colorBits);
    UpdateToolbar();
    return true;
}

// Called when the view window is created.
int CView::OnCreate(CREATESTRUCT&)
{
    // Create horizontal pager control.
    m_pager.Create(*this);
    DWORD style = m_pager.GetStyle();
    style |= PGS_HORZ;
    m_pager.SetStyle(style);

    // Create the transparent toolbar with a no resizing style.
    m_toolbar.Create(m_pager);
    style = m_toolbar.GetStyle();
    style |= CCS_NORESIZE;
    m_toolbar.SetStyle(style);

    // Assign the toolbar to the pager control.
    m_pager.SetChild(m_toolbar.GetHwnd());
    m_pager.SetBkColor(RGB(122,232,232));  // Color of left, right pager buttons.

    return 0;
}

// Called after the parent receives a DPI changed message.
LRESULT CView::OnDpiChanged(UINT, WPARAM, LPARAM)
{
    UpdateToolbar();
    RecalcLayout();
    return 0;
}

// OnDraw is called when part or all of the window needs to be redrawn.
void CView::OnDraw(CDC& dc)
{
    // Use a memory DC for double buffering.
    CMemDC dcClient(dc);
    CRect rcClient = GetClientRect();
    int cx = rcClient.Width();
    int cy = rcClient.Height();
    dcClient.CreateCompatibleBitmap(dc, cx, cy);

    // Use the message font for Windows 7 and higher.
    if (GetWinVersion() >= 2601)
    {
        NONCLIENTMETRICS info = GetNonClientMetrics();
        LOGFONT lf = DpiScaleLogfont(info.lfMessageFont, 10);
        dcClient.CreateFontIndirect(lf);
    }

    COLORREF background = RGB(240, 240, 240);
    dcClient.FillRect(GetClientRect(), CBrush(background));

    // Centre some text in our view window.
    dcClient.SetBkColor(background);
    CString sizeInfo;
    int rowHeight = DpiScaleInt(20);
    CSize sz = m_toolbarImages.GetIconSize();
    sizeInfo << "Toolbar image size: cx = " << sz.cx << ", cy = " << sz.cy;
    dcClient.TextOut(0, 0, sizeInfo, -1);

    CString numberInfo;
    int row = 1;
    numberInfo << "Number of images in toolbar: " << m_toolbarImages.GetImageCount();
    dcClient.TextOut(0, rowHeight * row++, numberInfo, -1);

    CString colorDepth;
    colorDepth << "Bits per Pixel: " << m_colorBits;
    dcClient.TextOut(0, rowHeight * row++, colorDepth, -1);

    CString selected;
    selected << "Images selected: " << GetPressedButtons();
    dcClient.TextOut(0, rowHeight * row++, selected, -1);

    if (m_colorBits <= 24)
    {
        CString mask;
        mask << "Mask RGB color: " << GetRValue(m_mask) << "," << GetGValue(m_mask) << "," << GetBValue(m_mask);
        mask << (IsMasked() ? "   Enabled" : "   Disabled");
        dcClient.TextOut(0, rowHeight * row++, mask, -1);
    }

    int dpi = GetWindowDpi(*this);
    int scale = std::max(1, dpi / USER_DEFAULT_SCREEN_DPI);
    if (scale != 1)
    {
        CString dpiScaled;
        dpiScaled << "DPI scaling for high resolution monitor: " << scale << " x 1";
        dcClient.TextOut(0, rowHeight * row++, dpiScaled, -1);
    }

    dc.BitBlt(0, 0, cx, cy, dcClient, 0, 0, SRCCOPY);
}

// OnInitialUpdate is called immediately after the window is created.
void CView::OnInitialUpdate()
{
    // Load the initial toolbar bitmap from the bitmap resource.
    CreateImageList(32, 32, 32);
    m_toolbarImages.Add((HBITMAP)GetApp()->LoadImage(
        IDB_TOOLBAR32, IMAGE_BITMAP, 0, 0), m_mask);

    // Set the toolbar background color.
    SetBackground(RGB(224, 224, 224));
    UpdateToolbar();
}

// Process notification messages (WM_NOTIFY) sent by child windows
LRESULT CView::OnNotify(WPARAM wparam, LPARAM lparam)
{
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    switch (pHeader->code)
    {
    // PGN_CALCSIZE is a Notification sent by a pager control to obtain
    // the scrollable dimensions of the contained window.
    case PGN_CALCSIZE:
    {
        LPNMPGCALCSIZE   pCalcSize = (LPNMPGCALCSIZE)lparam;
        switch (pCalcSize->dwFlag)
        {
        // Used by the pager control to calculate the scrollable area of the
        // contained window.
        case PGF_CALCWIDTH:
        {
            // Get the optimum width of the toolbar.
            CSize size = m_toolbar.GetMaxSize();
            pCalcSize->iWidth = size.cx;
        }
        }
        break;
    }
    case PGN_SCROLL:
    {
        LPNMPGSCROLL pScroll = (LPNMPGSCROLL)lparam;
        pScroll->iScroll = m_toolbar.GetButtonSize().cx;
        break;
    }
    }

    // Some notifications should return a value when handled
    return CWnd::OnNotify(wparam, lparam);
}

// Called after the window's size has changed.
LRESULT CView::OnSize(UINT, WPARAM, LPARAM)
{
    RecalcLayout();
    return 0;
}

// Sets the CREATESTRUCT parameters before the window is created.
void CView::PreCreate(CREATESTRUCT& cs)
{
    // Adds a border with a sunken edge to view window.
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

// Reposition the child windows of the frame.
void CView::RecalcLayout()
{
    // Position the toolbar at the bottom of the view.
    if (m_toolbar.IsWindow() && m_toolbarImages.GetHandle())
    {
        m_toolbar.Autosize();
        CSize imageSize = m_toolbar.GetImageList().GetIconSize();

        CRect rc = GetClientRect();
        int x = 0;
        int y = rc.bottom - m_toolbar.GetMaxSize().cy;
        int height = m_toolbar.GetMaxSize().cy;
        m_pager.SetWindowPos(HWND_TOP, x, y, rc.Width(), height, SWP_SHOWWINDOW);
        m_pager.SetButtonSize(imageSize.cx/2);
    }
}

// Increase or reduce the bitmap size by the specified scale amounts.
CBitmap CView::ResizeBitmap(const CBitmap& bitmap, double xScale, double yScale)
{
    assert(bitmap.GetHandle() != nullptr);
    assert(xScale != 0);
    assert(yScale != 0);

    // Select the bitmap into a memory DC.
    CMemDC imageDC(nullptr);
    imageDC.SelectObject(bitmap);

    // Create and select a DIB using CreateDIBSection.
    CMemDC newImageDC(nullptr);
    BITMAP  bm = bitmap.GetBitmapData();
    bm.bmWidth = LONG(bm.bmWidth * xScale);
    bm.bmHeight = LONG(bm.bmHeight * yScale);
    CBitmapInfoPtr pbmi(bm);
    newImageDC.CreateDIBSection(newImageDC, pbmi, DIB_RGB_COLORS, nullptr,
        nullptr, 0);

    // Copy the color table from the vertical to the horizontal DIB.
    if (bm.bmBitsPixel <= 8)
    {
        int numColors = 1 << bm.bmBitsPixel;
        std::vector<RGBQUAD> colors(numColors);
        RGBQUAD* colorTable = colors.data();
        ::GetDIBColorTable(imageDC, 0, numColors, colorTable);
        ::SetDIBColorTable(newImageDC, 0, numColors, colorTable);
    }

    // Resize the bitmap to the new image size.
    CSize szImage = bitmap.GetSize();
    newImageDC.StretchBlt(0, 0, bm.bmWidth, bm.bmHeight, imageDC, 0, 0,
        szImage.cx, szImage.cy, SRCCOPY);

    return newImageDC.DetachBitmap();
}

// Saves the toolbar bitmap stored in the image list to the specified file.
bool CView::SaveToolbarImage(const CString& fileName)
{
    CBitmap horizontal = GetBitmapFromImageList();
    return WriteBitmapToFile(horizontal, fileName);
}

// Set the background color of the toolbar.
void CView::SetBackground(COLORREF background)
{
    m_background.CreateSolidBrush(background);
    SetClassLongPtr(GCLP_HBRBACKGROUND,
        reinterpret_cast<LONG_PTR>(m_background.GetHandle()));
    UpdateToolbar();
}

// Set the color of the transparency mask.
void CView::SetMaskColor(COLORREF mask)
{
    m_mask = mask;
    UpdateToolbar();
}

// Assigns a toolbar button for each image in the image list.
void CView::SetToolbarButtons()
{
    // Remove all toolbar buttons.
    while (m_toolbar.GetButtonCount() > 0)
    {
        m_toolbar.DeleteButton(0);
    }

    // Add a button for each image.
    int nButtons = m_toolbarImages.GetImageCount();
    for (int i = 0; i < nButtons; i++)
    {
        m_toolbar.AddButton(i + TB_BUTTON0);
        m_toolbar.SetButtonStyle(i + TB_BUTTON0, BTNS_CHECK);
    }
}

// Set the toolbar image list from m_toolbarImages. Uses m_mask as
// the transparency color if m_isMasked is true.
void CView::SetToolbarImageList()
{
    CBitmap bitmap = GetBitmapFromImageList();

    int dpi = GetWindowDpi(*this);
    int scale = std::max(1, dpi / USER_DEFAULT_SCREEN_DPI);
    if (scale > 1)
        bitmap = DpiScaleUpBitmap(bitmap);

    CSize sz = bitmap.GetSize();
    CImageList dpiImageList;

    // Support 16x16, 24x24, 32x32, 48x48 etc. Also support size 16x15.
    dpiImageList.Create(std::max(sz.cy, LONG(16* scale)), sz.cy, m_colorBits |
        ILC_MASK, 1, 1);

    if (m_isMasked)
        dpiImageList.Add(bitmap, m_mask);
    else
        dpiImageList.Add(bitmap);

    m_toolbar.SetImageList(dpiImageList);
    RecalcLayout();
}

// Updates the toolbar with the new image.
void CView::UpdateToolbar()
{
    SetToolbarImageList();
    SetToolbarButtons();
    RecalcLayout();
    Invalidate();
}

// Enable or disables the use of the color mask.
bool CView::SetMaskState(bool isMasked)
{
    m_isMasked = isMasked;
    SetToolbarImageList();
    Invalidate();
    return true;
}

// All window messages for this window pass through WndProc.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:                   return OnSize(msg, wparam, lparam);
        case WM_DPICHANGED_AFTERPARENT: return OnDpiChanged(msg, wparam, lparam);
        }

        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}

// Writes the bitmap to the specified file.
bool CView::WriteBitmapToFile(const CBitmap& bitmap, const CString& fileName)
{
    try
    {
        // Create our LPBITMAPINFO object.
        CBitmapInfoPtr pbmi(bitmap);

        // Create the reference DC for GetDIBits to use.
        CMemDC diBitDC(nullptr);

        // Use GetDIBits to create a DIB from our bitmap, and extract the colour data.
        VERIFY(diBitDC.GetDIBits(bitmap, 0, pbmi->bmiHeader.biHeight, nullptr,
            pbmi, DIB_RGB_COLORS));
        std::vector<byte> byteArray(pbmi->bmiHeader.biSizeImage, 0);
        byte* pByteArray = byteArray.data();
        VERIFY(diBitDC.GetDIBits(bitmap, 0, pbmi->bmiHeader.biHeight,
            pByteArray, pbmi, DIB_RGB_COLORS));

        // Reassign biClrUsed, as it is cleared by GetDIBits.
        if (pbmi->bmiHeader.biBitCount < 24)
            pbmi->bmiHeader.biClrUsed = (1U << pbmi->bmiHeader.biBitCount);

        // Declare and fill the BITMAPINFOHEADER and BITMAPFILEHEADER structs.
        LPBITMAPINFOHEADER pbmih = &pbmi->bmiHeader;
        BITMAPFILEHEADER hdr{};
        hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
        hdr.bfSize = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + pbmih->biSize +
            pbmih->biClrUsed * sizeof(RGBQUAD) + pbmih->biSizeImage);
        hdr.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) +
            pbmih->biSize + pbmih->biClrUsed * sizeof(RGBQUAD));

        // Write the bitmap to the specified file.
        CFile file(fileName, OPEN_ALWAYS);
        file.Write(&hdr, sizeof(BITMAPFILEHEADER));
        file.Write(pbmih, sizeof(BITMAPINFOHEADER) + pbmih->biClrUsed *
            sizeof(RGBQUAD));
        file.Write(pByteArray, pbmih->biSizeImage);

        return true;
    }

    catch (const CFileException& e)
    {
        CString str = CString("Failed to save file: ") + e.GetFilePath();
        MessageBox(str, AtoW(e.what()), MB_OK);
    }

    return false;
}
