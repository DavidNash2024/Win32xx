/////////////////////////////
// D2DView.cpp
//

#include "pch.h"
#include "D2DApp.h"
#include "D2DView.h"

////////////////////////////////
// CD2DView function definitions
//

// Constructor
CD2DView::CD2DView()
{
}

// Destructor
 CD2DView::~CD2DView()
{
    DiscardDeviceResources();
}

// Create the persistent Factory.
HRESULT CD2DView::CreateDeviceIndependentResources()
{
    if (!m_pDirect2dFactory)
    {
        return D2D1CreateFactory<ID2D1Factory>(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
    }
    return S_OK;
}

// Create the transient rendering pipe using our own internal member factory
HRESULT CD2DView::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (!m_pRenderTarget)
    {
        CRect rc = GetClientRect();
        D2D1_SIZE_U size = D2D1::SizeU(rc.Width(), rc.Height());

        // Create a Direct2D render target.
        hr = m_pDirect2dFactory->CreateHwndRenderTarget(
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

// Clear the GPU resources.
void CD2DView::DiscardDeviceResources()
{
    m_pRenderTarget.Reset();
    m_pLightSlateGrayBrush.Reset();
    m_pCornflowerBlueBrush.Reset();
}

int CD2DView::OnCreate(CREATESTRUCT&)
{
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Start the Direct2D framework for this window instance.
    if (FAILED(CreateDeviceIndependentResources()))
    {
        ::MessageBox(*this, L"Failed to initialize Direct2D Factory", L"Error", MB_OK | MB_ICONERROR);
        return -1;
    }

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
// This function is only called when using Per Monitor DPI Awareness,
// version 2. This is specified by loading the appropriate manifest in the
// resource script (resource.rc).
LRESULT CD2DView::OnDpiChanged(UINT, WPARAM, LPARAM lparam)
{
    LPRECT prc = reinterpret_cast<LPRECT>(lparam);
    SetWindowPos(HWND_TOP, *prc, SWP_SHOWWINDOW);
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
    float zoom = static_cast<float>(GetWindowDpi(*this)) / static_cast<float>(GetWindowDpi(HWND_DESKTOP));
    HRESULT hr = CreateDeviceResources();
    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
        float width = rtSize.width;
        float height = rtSize.height;

        // Vertical Grid
        for (float x = 0.0f; x < width; x += 16.0f * zoom)
        {
            m_pRenderTarget->DrawLine(D2D1::Point2F(x, 0.0f), D2D1::Point2F(x, height), m_pLightSlateGrayBrush.Get(), zoom);
        }

        // Horizontal Grid
        for (float y = 0.0f; y < height; y += 16.0f * zoom)
        {
            m_pRenderTarget->DrawLine(D2D1::Point2F(0.0f, y), D2D1::Point2F(width, y), m_pLightSlateGrayBrush.Get(), zoom);
        }

        D2D1_RECT_F rectangle1 = D2D1::RectF(
            rtSize.width / 2.0f - 40.0f * zoom, rtSize.height / 2.0f - 40.0f * zoom,
            rtSize.width / 2.0f + 40.0f * zoom, rtSize.height / 2.0f + 40.0f * zoom
        );

        D2D1_RECT_F rectangle2 = D2D1::RectF(
            rtSize.width / 2.0f - 80.0f * zoom, rtSize.height / 2.0f - 80.0f * zoom,
            rtSize.width / 2.0f + 80.0f * zoom, rtSize.height / 2.0f + 80.0f * zoom
        );

        // Draw a filled rectangle.
        m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush.Get());

        // Draw the outline of a rectangle.
        m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush.Get(), 4.0f * zoom);
        hr = m_pRenderTarget->EndDraw();
    }

    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardDeviceResources();
        Invalidate();
    }

    return hr;
}

// Resize the render target when the window is resized.
void CD2DView::OnResize(UINT width, UINT height)
{
    if (m_pRenderTarget)
    {
        m_pRenderTarget->Resize(D2D1::SizeU(width, height));
    }
}

// Called when the window is resized.
LRESULT CD2DView::OnSize(UINT, WPARAM, LPARAM lparam)
{
    OnResize(LOWORD(lparam), HIWORD(lparam));
    return 0;
}

// Specify the initial window size.
void CD2DView::PreCreate(CREATESTRUCT& cs)
{
    cs.x = DpiScaleInt(80);
    cs.y = DpiScaleInt(80);
    cs.cx = DpiScaleInt(640);
    cs.cy = DpiScaleInt(480);
}

void CD2DView::PreRegisterClass(WNDCLASS& wc)
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = ::LoadCursor(nullptr, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)::GetStockObject(NULL_BRUSH);
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
        case WM_DPICHANGED:     return OnDpiChanged(msg, wparam, lparam);
        case WM_SIZE:           return OnSize(msg, wparam, lparam);

        default: return WndProcDefault(msg, wparam, lparam);
        }
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << L"Error: " << e.what();
        TaskDialogBox(nullptr, str1, str2, TD_ERROR_ICON);
    }

    return 0;
}
