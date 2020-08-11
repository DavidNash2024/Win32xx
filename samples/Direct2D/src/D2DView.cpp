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

// Perform the drawing.
HRESULT CD2DView::OnRender()
{
    HRESULT hr = S_OK;

    hr = CreateDeviceResources();
    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
        int width = static_cast<int>(rtSize.width);
        int height = static_cast<int>(rtSize.height);

        // Draw horizontal lines
        for (int x = 0; x < width; x += 10)
        {
            m_pRenderTarget->DrawLine(
                D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
                D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
                m_pLightSlateGrayBrush,
                0.5f
            );
        }

        // Draw vertical lines
        for (int y = 0; y < height; y += 10)
        {
            m_pRenderTarget->DrawLine(
                D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
                D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
                m_pLightSlateGrayBrush,
                0.5f
            );
        }

        // Draw two rectangles.
        D2D1_RECT_F rectangle1 = D2D1::RectF(
            rtSize.width / 2 - 50.0f,
            rtSize.height / 2 - 50.0f,
            rtSize.width / 2 + 50.0f,
            rtSize.height / 2 + 50.0f
        );

        D2D1_RECT_F rectangle2 = D2D1::RectF(
            rtSize.width / 2 - 100.0f,
            rtSize.height / 2 - 100.0f,
            rtSize.width / 2 + 100.0f,
            rtSize.height / 2 + 100.0f
        );

        // Draw a filled rectangle.
        m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

        // Draw the outline of a rectangle.
        m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);
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

// Specify the initial window size.
void CD2DView::PreCreate(CREATESTRUCT&cs)
{
    cs.cx = 640;
    cs.cy = 480;
}

// Set the WNDCLASS parameters before the window is created.
void CD2DView::PreRegisterClass(WNDCLASS& wc)
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = ::LoadCursor(NULL, IDI_APPLICATION);
    wc.lpszClassName = _T("Direct2D");
}

// Called when the window is created.
int CD2DView::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

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

// Called when part of the window needs to be redrawn.
LRESULT CD2DView::OnPaint(UINT, WPARAM, LPARAM)
{
    OnRender();
    ValidateRect();

    return 0;
}

// Called when the window is resized.
LRESULT CD2DView::OnSize(UINT, WPARAM, LPARAM lparam)
{
    UINT width = LOWORD(lparam);
    UINT height = HIWORD(lparam);
    OnResize(width, height);

    return 0;
}

// Called when the display resolution has changed.
LRESULT CD2DView::OnDisplayChange(UINT, WPARAM, LPARAM)
{
    Invalidate();
    return 0;
}

// Process the window messages.
LRESULT CD2DView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DISPLAYCHANGE:  return OnDisplayChange(msg, wparam, lparam);
    case WM_SIZE:           return OnSize(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}
