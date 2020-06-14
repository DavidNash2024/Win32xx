
#include "stdafx.h"

#ifdef _MSC_VER
#pragma warning (disable : 4458) // disable declaration hides class member warning
#endif
#include <gdiplus.h>
#ifdef _MSC_VER
#pragma warning (default : 4458) // return warning to default
#endif

#include "CoverImage.h"

using namespace Gdiplus;

// Construtor. 
CCoverImage::CCoverImage()
{
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    // The entry for the dialog's control in resource.rc must match this name.
    CString className = L"CoverImage";

    // Register the window class for the dialog's picture control
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));

    if (!::GetClassInfo(GetApp()->GetInstanceHandle(), className, &wc))
    {
        wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
        wc.lpszClassName = className;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.hInstance = GetApp()->GetInstanceHandle();
        ::RegisterClass(&wc);
    }

    assert(::GetClassInfo(GetApp()->GetInstanceHandle(), className, &wc));
}

// Destructor.
CCoverImage::~CCoverImage()
{
}

// Draws the cover image to the specified device context.
void CCoverImage::DrawImage(CDC& dc)
{
    // Convert the image string to binary
    if (m_imageData.size() > 0)
    {
        UINT len = (UINT)m_imageData.size();
        HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, len);
        if (mem != NULL)
        {
            BYTE* pMem = (BYTE*)GlobalLock(mem);
            if (pMem != NULL)
            {
                memcpy(pMem, &m_imageData[0], len);
                IStream* stream = NULL;
                VERIFY(S_OK == CreateStreamOnHGlobal(mem, FALSE, &stream));
                Image imgTest(stream);

                // Draw the image
                UINT width = GetClientRect().Width();
                UINT height = GetClientRect().Height();
                Rect destRect(0, 0, width, height);
                Graphics graphics(dc);
                graphics.DrawImage(&imgTest, destRect);

                // Cleanup
                stream->Release();
                GlobalUnlock(mem);
            }
            GlobalFree(mem);
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

// Called when the CCoverImage window needs to be redrawn.
void CCoverImage::OnDraw(CDC& dc)
{
    DrawImage(dc);
}

// Calls OnDraw to preform painting for this custom control
LRESULT CCoverImage::OnPaint(UINT, WPARAM, LPARAM)
{
    if (::GetUpdateRect(*this, NULL, FALSE))
    {
        CPaintDC dc(*this);
        OnDraw(dc);
    }
    else
    // RedrawWindow can require repainting without an update rect
    {
        CClientDC dc(*this);
        OnDraw(dc);
    }

    // No more drawing required
    return 0;
}

// Process the window messages for the CCoverImage window.
LRESULT CCoverImage::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_SIZE:
        {
            CDC dc = GetDC();
            OnDraw(dc);
        }
        break;
    }

    return WndProcDefault(msg, wparam, lparam);
}
