/////////////////////////////
// D2DView.cpp
//

#include "stdafx.h"
#include "D2DApp.h"
#include "D2DView.h"

////////////////////////////////
// CD2DView function definitions
//

// Constructor
CD2DView::CD2DView() : m_pRenderTarget(NULL), m_pLightSlateGrayBrush(NULL), m_pCornflowerBlueBrush(NULL)
{
}

// Destructor
 CD2DView::~CD2DView()
{
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
}

// Create the resources used by OnRender
HRESULT CD2DView::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (!m_pRenderTarget)
    {
        CRect rc = GetClientRect();

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
        );

        // Create a Direct2D render target.
        hr = GetD2DFactory()->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(*this, size),
            &m_pRenderTarget
        );

        if (SUCCEEDED(hr))
        {
            // Create a gray brush.
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::LightSlateGray),
                &m_pLightSlateGrayBrush
            );
        }
        if (SUCCEEDED(hr))
        {
            // Create a blue brush.
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
                &m_pCornflowerBlueBrush
            );
        }
    }

    return hr;
}

// Release memory allocated to resources.
void CD2DView::DiscardDeviceResources()
{
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
}

// Called when the window is created.
int CD2DView::OnCreate(CREATESTRUCT&)
{
    // Set the window's icon
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    return 0;
}

// Called when the window is destroyed.
void CD2DView::OnDestroy()
{
    // End the application.
    PostQuitMessage(0);
}

// Called when the display resolution has changed.
LRESULT CD2DView::OnDisplayChange(UINT, WPARAM, LPARAM)
{

    Invalidate();
    return 0;
}

// Called when the effective dots per inch (dpi) for a window has changed.
// This occurs when:
//  - The window is moved to a new monitor that has a different DPI.
//  - The DPI of the monitor hosting the window changes.
LRESULT CD2DView::OnDPIChanged(UINT, WPARAM, LPARAM lparam)
{
    LPRECT prc = (LPRECT)lparam;
    SetWindowPos(0, *prc, SWP_SHOWWINDOW);
    return 0;
}

// Called when part of the window needs to be redrawn.
LRESULT CD2DView::OnPaint(UINT, WPARAM, LPARAM)
{
    OnRender();
    ValidateRect();

    return 0;
}

// Perform the drawing.
HRESULT CD2DView::OnRender()
{
    float zoom = static_cast<float>(GetWindowDPI(*this)) / static_cast<float>(GetWindowDPI(0));
    HRESULT hr = CreateDeviceResources();
    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
        float width = rtSize.width;
        float height = rtSize.height;

        // Draw horizontal lines
        for (float x = 0.0f; x < width; x += 16.0f * zoom)
        {
            m_pRenderTarget->DrawLine(
                D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
                D2D1::Point2F(static_cast<FLOAT>(x), height),
                m_pLightSlateGrayBrush,
                zoom
            );
        }

        // Draw vertical lines
        for (float y = 0.0f; y < height; y += 16.0f * zoom)
        {
            m_pRenderTarget->DrawLine(
                D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
                D2D1::Point2F(width, static_cast<FLOAT>(y)),
                m_pLightSlateGrayBrush,
                zoom
            );
        }

        // Draw two rectangles.
        D2D1_RECT_F rectangle1 = D2D1::RectF(
            rtSize.width / (2.0f) - 40.0f * zoom,
            rtSize.height / (2.0f) - 40.0f * zoom,
            rtSize.width / (2.0f) + 40.0f * zoom,
            rtSize.height / (2.0f) + 40.0f * zoom
        );

        D2D1_RECT_F rectangle2 = D2D1::RectF(
            rtSize.width / (2.0f) - 80.0f * zoom,
            rtSize.height / (2.0f) - 80.0f * zoom,
            rtSize.width / (2.0f) + 80.0f * zoom,
            rtSize.height / (2.0f) + 80.0f * zoom
        );

        // Draw a filled rectangle.
        m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

        // Draw the outline of a rectangle.
        m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush, 4 * zoom);
        hr = m_pRenderTarget->EndDraw();
    }

    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardDeviceResources();
    }

    return hr;
}

// Resize the render target when the window is resized.
void CD2DView::OnResize(UINT width, UINT height)
{
    if (m_pRenderTarget)
    {
        // Note: This method can fail, but it's okay to ignore the
        // error here, because the error will be returned again
        // the next time EndDraw is called.
        m_pRenderTarget->Resize(D2D1::SizeU(width, height));
    }
}

// Called when the window is resized.
LRESULT CD2DView::OnSize(UINT, WPARAM, LPARAM lparam)
{
    UINT width = LOWORD(lparam);
    UINT height = HIWORD(lparam);
    OnResize(width, height);

    return 0;
}

// Specify the initial window size.
void CD2DView::PreCreate(CREATESTRUCT&cs)
{
    cs.x = DPIScaleInt(80);                 // top x
    cs.y = DPIScaleInt(80);                 // top y
    cs.cx = DPIScaleInt(640);               // width
    cs.cy = DPIScaleInt(480);               // height
}

// Set the WNDCLASS parameters before the window is created.
void CD2DView::PreRegisterClass(WNDCLASS& wc)
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = ::LoadCursor(0, IDI_APPLICATION);
    wc.lpszClassName = L"Direct2D";
}

// Process the window messages.
LRESULT CD2DView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DISPLAYCHANGE:  return OnDisplayChange(msg, wparam, lparam);
        case WM_DPICHANGED:     return OnDPIChanged(msg, wparam, lparam);
        case WM_SIZE:           return OnSize(msg, wparam, lparam);
        }

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
