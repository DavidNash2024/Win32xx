/////////////////////////////
// DXView.cpp
//

#include "DXView.h"
#include "resource.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////
// Global helper function for compiling shaders with D3DCompile.
//
HRESULT CompileShaderFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, Microsoft::WRL::ComPtr<ID3DBlob>& blobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob;
    hr = ::D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, blobOut.ReleaseAndGetAddressOf(), reinterpret_cast<ID3DBlob**>(pErrorBlob.GetAddressOf()));

    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            Trace(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
        }
        return hr;
    }

    return S_OK;
}

///////////////////////////////
// CDXView function definitions
//

// Initializes the Direct3D 11 device and the immediate context.
HRESULT CDXView::CreateD3DDevice()
{
    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    HRESULT hr = S_OK;
    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        m_pd3dDevice.Reset();
        m_pImmediateContext.Reset();

        m_driverType = driverTypes[driverTypeIndex];
        hr = ::D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, m_pd3dDevice.GetAddressOf(), &m_featureLevel, m_pImmediateContext.GetAddressOf());

        if (hr == E_INVALIDARG)
        {
            m_pd3dDevice.Reset();
            m_pImmediateContext.Reset();

            hr = ::D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1],
                numFeatureLevels - 1, D3D11_SDK_VERSION, m_pd3dDevice.GetAddressOf(), &m_featureLevel, m_pImmediateContext.GetAddressOf());
        }

        if (SUCCEEDED(hr))
            break;
    }
    return hr;
}

// Creates and binds a render target view from the swap chain's back buffer,
// and configures the rasterizer viewport for rendering.
HRESULT CDXView::CreateRenderTarget(UINT width, UINT height)
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    HRESULT hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()));
    if (FAILED(hr)) return hr;

    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf());
    if (FAILED(hr)) return hr;

    m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp{};
    vp.Width = static_cast<FLOAT>(width);
    vp.Height = static_cast<FLOAT>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    return S_OK;
}

// Creates a window-associated DXGI swap chain from a Direct3D 11.1 interface path.
// Falls back to Direct3D 11.0 if unsupported.
HRESULT CDXView::CreateSwapChain(Microsoft::WRL::ComPtr<IDXGIFactory1>& factory, UINT width, UINT height)
{
    Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory2;
    HRESULT hr = factory.As(&dxgiFactory2);

    if (SUCCEEDED(hr))
    {
        // Direct3D 11.1 Path
        m_pd3dDevice.As(&m_pd3dDevice1);
        m_pImmediateContext.As(&m_pImmediateContext1);

        DXGI_SWAP_CHAIN_DESC1 sd{};
        sd.Width = width;
        sd.Height = height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd(m_pd3dDevice.Get(), *this, &sd, nullptr, nullptr, m_pSwapChain1.GetAddressOf());
        if (SUCCEEDED(hr))
        {
            hr = m_pSwapChain1.As(&m_pSwapChain);
        }
    }
    else
    {
        // Direct3D 11.0 Path Fallback
        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = *this;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = factory->CreateSwapChain(m_pd3dDevice.Get(), &sd, m_pSwapChain.GetAddressOf());
    }
    return hr;
}

// Retrieves the DXGI factory from the internal Direct3D device.
HRESULT CDXView::GetDXGIFactory(Microsoft::WRL::ComPtr<IDXGIFactory1>& factory)
{
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    HRESULT hr = m_pd3dDevice.As(&dxgiDevice);
    if (SUCCEEDED(hr))
    {
        Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
        hr = dxgiDevice->GetAdapter(adapter.GetAddressOf());
        if (SUCCEEDED(hr))
        {
            hr = adapter->GetParent(IID_PPV_ARGS(factory.ReleaseAndGetAddressOf()));
        }
    }
    return hr;
}

// Initialize the coordinate space transforms.
void CDXView::InitCamera(UINT width, UINT height)
{
    XMVECTOR Eye = XMVectorSet(0.0f, 1.5f, -8.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    ::XMStoreFloat4x4(&m_view, XMMatrixLookAtLH(Eye, At, Up));

    FLOAT aspectRatio = width / static_cast<FLOAT>(height);
    ::XMStoreFloat4x4(&m_projection, XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, 0.01f, 100.0f));
}

// Setup DirectX for the window.
HRESULT CDXView::InitDevice()
{
    // High resolution timing tracking.
    ::QueryPerformanceFrequency(&m_frequency);
    ::QueryPerformanceCounter(&m_lastTime);

    // Device creation.
    HRESULT hr = CreateD3DDevice();
    if (FAILED(hr)) return hr;

    // Extract factory from device pointers.
    Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
    hr = GetDXGIFactory(dxgiFactory);
    if (FAILED(hr)) return hr;

    // Setup presentation infrastructure.
    CRect rc = GetClientRect();
    hr = CreateSwapChain(dxgiFactory, rc.Width(), rc.Height());
    dxgiFactory->MakeWindowAssociation(*this, DXGI_MWA_NO_ALT_ENTER);
    if (FAILED(hr)) return hr;

    // Bind output targets & create drawing area.
    hr = CreateRenderTarget(rc.Width(), rc.Height());
    if (FAILED(hr)) return hr;

    // Load layout and GPU instructions.
    hr = InitShaders();
    if (FAILED(hr)) return hr;

    // Allocate drawing topology resources.
    hr = InitGeometry();
    if (FAILED(hr)) return hr;

    // Matrix spatial projection mapping.
    InitCamera(rc.Width(), rc.Height());

    return S_OK;
}

// Shader and input layout setup.
HRESULT CDXView::InitShaders()
{
    // Compile and create Vertex Shader.
    Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob;
    HRESULT hr = ::CompileShaderFromFile(L"DirectX11.fx", "VS", "vs_4_0", pVSBlob);
    if (FAILED(hr))
    {
        ::MessageBox(*this, L"The FX file cannot be compiled.", L"Error", MB_OK | MB_ICONERROR);
        return hr;
    }

    hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf());
    if (FAILED(hr))
    {
        return hr;
    }

    // Define and create Input Layout.
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = m_pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), m_pVertexLayout.GetAddressOf());

    if (FAILED(hr)) return hr;

    m_pImmediateContext->IASetInputLayout(m_pVertexLayout.Get());

    // Compile and create Pixel Shader.
    Microsoft::WRL::ComPtr<ID3DBlob> pPSBlob;
    hr = ::CompileShaderFromFile(L"DirectX11.fx", "PS", "ps_4_0", pPSBlob);
    if (FAILED(hr)) return hr;

    hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf());

    return hr;
}

// Mesh and Buffer Allocation.
HRESULT CDXView::InitGeometry()
{
    SimpleVertex vertices[] =
    {
        { XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
    };

    // Vertex Buffer.
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * ARRAYSIZE(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA InitData{};
    InitData.pSysMem = vertices;
    HRESULT hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, m_pVertexBuffer.GetAddressOf());
    if (FAILED(hr)) return hr;

    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, reinterpret_cast<ID3D11Buffer* const*>(m_pVertexBuffer.GetAddressOf()), &stride, &offset);

    // Index Buffer.
    WORD indices[] =
    {
        3,1,0, 2,1,3,
        0,5,4, 1,5,0,
        3,4,7, 0,4,3,
        1,6,5, 2,6,1,
        2,7,6, 3,7,2,
        6,4,5, 7,4,6
    };
    bd.ByteWidth = sizeof(WORD) * ARRAYSIZE(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    InitData.pSysMem = indices;
    hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, m_pIndexBuffer.GetAddressOf());
    if (FAILED(hr)) return hr;

    m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Constant Buffer.
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    return m_pd3dDevice->CreateBuffer(&bd, nullptr, m_pConstantBuffer.GetAddressOf());
}

// OnCreate is called when the window is created.
int CDXView::OnCreate(CREATESTRUCT&)
{
    if (FAILED(InitDevice()))
    {
        ::MessageBox(*this, L"Failed to initialize DirectX 11 device configuration.",
            L"DirectX Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Set the window's icon.
    SetIconSmall(IDI_DIRECTX11);
    SetIconLarge(IDI_DIRECTX11);

    // Set the window title.
    SetWindowText(LoadString(IDS_APP_TITLE));

    TRACE("OnCreate\n");
    return 0;
}

// Called when the window is destroyed.
void CDXView::OnDestroy()
{
    ::PostQuitMessage(0);
}

// Called when the window is repainted.
LRESULT CDXView::OnPaint(UINT, WPARAM, LPARAM)
{
    Render();
    ValidateRect();
    return 0;
}

// Called when the window is resized.
LRESULT CDXView::OnSize(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (wParam != SIZE_MINIMIZED)
    {
        CRect rc = GetClientRect();
        UINT width = rc.Width();
        UINT height = rc.Height();

        if (width > 0 && height > 0)
        {
            ResizeBuffers(width, height);
            InitCamera(width, height);
            Render();
        }
    }

    return FinalWindowProc(msg, wParam, lParam);
}

// Called while the window is being resized.
LRESULT CDXView::OnSizing(UINT msg, WPARAM wParam, LPARAM lParam)
{
    Invalidate();

    return FinalWindowProc(msg, wParam, lParam);
}

// Sets the CREATESTRUCT parameters before the window is created.
void CDXView::PreCreate(CREATESTRUCT& cs)
{
    cs.cx = DpiScaleInt(640);
    cs.cy = DpiScaleInt(480);
    cs.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
}

// Draw the rotated cube in the window.
void CDXView::Render()
{
    if (!m_pImmediateContext || !m_pRenderTargetView)
        return;

    // Clear the back buffer frame view.
    float ClearColor[] = { 0.0f, 0.125f, 0.5f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), ClearColor);

    // Calculate the time based rotation.
    LARGE_INTEGER currentTime;
    ::QueryPerformanceCounter(&currentTime);

    float deltaTime = static_cast<float>(currentTime.QuadPart - m_lastTime.QuadPart) /
        static_cast<float>(m_frequency.QuadPart);
    m_lastTime = currentTime;

    // Rotate at 1.5 radian per second.
    m_rotationAngle += deltaTime * 1.5f;
    ::XMStoreFloat4x4(&m_world, XMMatrixRotationY(m_rotationAngle));

    // Package constant buffer parameters using matrix transposition registers.
    ConstantBuffer cb;
    cb.mWorld = ::XMMatrixTranspose(XMLoadFloat4x4(&m_world));
    cb.mView = ::XMMatrixTranspose(XMLoadFloat4x4(&m_view));
    cb.mProjection = ::XMMatrixTranspose(XMLoadFloat4x4(&m_projection));

    m_pImmediateContext->UpdateSubresource(m_pConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);

    // Set shaders, layouts, and constants into state context pipeline.
    m_pImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, reinterpret_cast<ID3D11Buffer* const*>(m_pConstantBuffer.GetAddressOf()));
    m_pImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

    // Draw geometry call.
    m_pImmediateContext->DrawIndexed(36, 0, 0);

    // Present rendering frame back buffer loop swap.
    m_pSwapChain->Present(1, 0);
}

// Updates the swap chain's back buffer size, format, and number of buffers.
// This should be called when the application window is resized.
HRESULT CDXView::ResizeBuffers(UINT width, UINT height)
{
    if (!m_pSwapChain) return S_OK;

    // Clear current pipeline targets to drop references to old buffers.
    if (m_pImmediateContext)
        m_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

    // Reset the old view so the swap chain can resize.
    m_pRenderTargetView.Reset();

    // Resize swap chain buffers (0 keeps current count and format).
    HRESULT hr = m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr)) return hr;

    // Re-extract back buffer texture.
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()));
    if (FAILED(hr)) return hr;

    // Recreate Render Target View.
    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf());
    if (FAILED(hr)) return hr;

    // Bind target back to Output Merger stage.
    m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), nullptr);

    // Update viewport mapping coordinates.
    D3D11_VIEWPORT vp{};
    vp.Width = static_cast<FLOAT>(width);
    vp.Height = static_cast<FLOAT>(height);
    vp.MaxDepth = 1.0f;
    m_pImmediateContext->RSSetViewports(1, &vp);

    return S_OK;
}

// The window procedure handles the window messages.
LRESULT CDXView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:   return OnSize(msg, wparam, lparam);
        case WM_SIZING: return OnSizing(msg, wparam, lparam);
        case WM_PAINT:  return OnPaint(msg, wparam, lparam);

        default: break;
        }
        return WndProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << L"Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    return 0;
}
