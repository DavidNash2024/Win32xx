/////////////////////////////
// CoverImage.cpp
//

#include "stdafx.h"

#if defined (_MSC_VER) && (_MSC_VER == 1900) // == VS2015
#pragma warning (disable : 4458) // disable warning: declaration hides class member.

// Declare min and max for older versions of Visual Studio.
using std::min;
using std::max;
#endif

#include <gdiplus.h>

#if defined (_MSC_VER) && (_MSC_VER == 1900) // == VS2015
#pragma warning (default : 4458) // return warning to default.
#endif

#include <shlwapi.h>

#include "CoverImage.h"

using namespace Gdiplus;

////////////////////////////////////
// CCoverImage function definitions.
//

// Constructor.
CCoverImage::CCoverImage()
{
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);

    // The entry for the dialog's control in resource.rc must match this name.
    CString className = L"CoverImage";

    // Register the window class for the dialog's picture control.
    WNDCLASS wc{};
    HINSTANCE instance = GetApp()->GetInstanceHandle();
    if (!::GetClassInfo(instance, className, &wc))
    {
        wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = className;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.hInstance = instance;
        ::RegisterClass(&wc);
    }

    assert(::GetClassInfo(instance, className, &wc));
}

// Destructor.
CCoverImage::~CCoverImage()
{
    GdiplusShutdown(m_gdiplusToken);
}

// Draws the cover image to the specified device context.
void CCoverImage::DrawImage(CDC& dc)
{
    // Convert the image string to binary.
    UINT  bufferSize = static_cast<UINT>(m_imageData.size());
    if (bufferSize > 0)
    {
        IStream* stream = ::SHCreateMemStream(m_imageData.data(), bufferSize);
        if (stream)
        {
            Image cover(stream);

            // Draw the image.
            UINT width = GetClientRect().Width();
            UINT height = GetClientRect().Height();
            Rect destRect(0, 0, width, height);
            Graphics graphics(dc);
            graphics.DrawImage(&cover, destRect);

            // Cleanup.
            stream->Release();
        }
    }
    else
    {
        CRect rc = GetWindowRect();
        SolidBrush solidBrush(Color(255, 255, 255, 255));       // white brush
        Rect rectangle(0, 0, rc.Width(), rc.Height());
        Graphics graphics(dc);
        graphics.FillRectangle(&solidBrush, rectangle);
    }
}

// Calls OnDraw to perform painting for this custom control.
LRESULT CCoverImage::OnPaint(UINT, WPARAM, LPARAM)
{
    CPaintDC dc(*this);
    DrawImage(dc);

    // No more drawing required.
    return 0;
}

// Process the window messages for the CCoverImage window.
LRESULT CCoverImage::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:
        {
            CClientDC dc = GetDC();
            OnDraw(dc);
        }
        break;
        }

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
        TaskDialogBox(nullptr, str1, L"Error: std::exception", TD_ERROR_ICON);
    }

    return 0;
}
