/////////////////////////////
// DXView.cpp
//

#include "DXView.h"
#include "resource.h"

// Safe release helper for raw COM interfaces.
template <class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != nullptr)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = nullptr;
    }
}

////////////////////////////////////////////////////////////////
// Global helper function for compiling shaders with D3DCompile.
//
HRESULT CompileShaderFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
            pErrorBlob->Release();
        }
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

///////////////////////////////
// CDXView function definitions
//

// CDXView Constructor.
CDXView::CDXView()
{
    XMStoreFloat4x4(&m_world, XMMatrixIdentity());
    XMStoreFloat4x4(&m_view, XMMatrixIdentity());
    XMStoreFloat4x4(&m_projection, XMMatrixIdentity());
}

// CDXView Destructor.
CDXView::~CDXView()
{
    CleanupDevice();
}

// Clean up the objects we've created.
void CDXView::CleanupDevice()
{
    if (m_pImmediateContext)
        m_pImmediateContext->ClearState();

    SafeRelease(&m_pConstantBuffer);
    SafeRelease(&m_pVertexBuffer);
    SafeRelease(&m_pIndexBuffer);
    SafeRelease(&m_pVertexLayout);
    SafeRelease(&m_pVertexShader);
    SafeRelease(&m_pPixelShader);
    SafeRelease(&m_pRenderTargetView);
    SafeRelease(&m_pSwapChain1);
    SafeRelease(&m_pSwapChain);
    SafeRelease(&m_pImmediateContext1);
    SafeRelease(&m_pImmediateContext);
    SafeRelease(&m_pd3dDevice1);
    SafeRelease(&m_pd3dDevice);
}

// Create Direct3D device and swap chain.
HRESULT CDXView::InitDevice()
{
    HRESULT hr = S_OK;

    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_lastTime);

    RECT rc;
    ::GetClientRect(*this, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);

        if (hr == E_INVALIDARG)
        {
            hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1],
                     numFeatureLevels - 1, D3D11_SDK_VERSION, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
        }

        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr))
        return hr;

    // Obtain DXGI factory from device.
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    // Create swap chain
    IDXGIFactory2* dxgiFactory2 = nullptr;
    hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
    if (SUCCEEDED(hr))
    {
        hr = m_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pd3dDevice1));
        if (SUCCEEDED(hr))
        {
            m_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pImmediateContext1));
        }

        DXGI_SWAP_CHAIN_DESC1 sd{};
        sd.Width = width;
        sd.Height = height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd(m_pd3dDevice, *this, &sd, nullptr, nullptr, &m_pSwapChain1);
        if (SUCCEEDED(hr))
        {
            hr = m_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems.
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

        hr = dxgiFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain);
    }

    // Note this sample doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation(*this, DXGI_MWA_NO_ALT_ENTER);

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;

    // Setup the render target view.
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    // Setup the viewport.
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    // Compile the vertex shader.
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile(L"DirectX11.fx", "VS", "vs_4_0", &pVSBlob);
    if (FAILED(hr))
    {
        ::MessageBox(*this, 
            L"The FX file cannot be compiled. Please place it in the application working folder.",
            L"Error", MB_OK | MB_ICONERROR);
        return hr;
    }

    // Create the vertex shader
    hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }

    // Define the input layout.
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout.
    hr = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &m_pVertexLayout);
    pVSBlob->Release();
    if (FAILED(hr)) return hr;

    // Set the input layout.
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

    // Compile the pixel shader.
    ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(L"DirectX11.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr)) return hr;

    // Create the pixel shader.
    hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr)) return hr;

    // Create vertex buffer for the cube.
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

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * ARRAYSIZE(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA InitData{};
    InitData.pSysMem = vertices;
    hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;

    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    // Declare the index buffer.
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
    hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
    if (FAILED(hr)) return hr;

    m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Constant Buffer Definition
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = m_pd3dDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);
    if (FAILED(hr)) return hr;

    // Setup initial View and Projection configurations
    XMVECTOR Eye = XMVectorSet(0.0f, 1.5f, -8.0f, 0.0f); // Changed Z from -5.0f to -8.0f, pushed Y up slightly
    XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMStoreFloat4x4(&m_view, XMMatrixLookAtLH(Eye, At, Up));
    XMStoreFloat4x4(&m_projection, XMMatrixPerspectiveFovLH(XM_PIDIV4, width / static_cast<FLOAT>(height), 0.01f, 100.0f));

    return S_OK;
}

// OnCreate is called when the window is created.
int CDXView::OnCreate(CREATESTRUCT&)
{
    if (FAILED(InitDevice()))
    {
        ::MessageBox(*this, L"Failed to initialize DirectX 11 device configuration.", L"DirectX Error", MB_OK | MB_ICONERROR);
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

// Sets the CREATESTRUCT parameters before the window is created.
void CDXView::PreCreate(CREATESTRUCT& cs)
{
    cs.cx = 640;
    cs.cy = 480;
    cs.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
}

// Called when the window is resized.
LRESULT CDXView::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);

    UINT width = LOWORD(lparam);
    UINT height = HIWORD(lparam);

    if (!m_pd3dDevice || !m_pSwapChain)
        return 0;

    // Flush active context references to the back buffer.
    m_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
    SafeRelease(&m_pRenderTargetView);

    // Resize swap chain buffers.
    HRESULT hr = m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (SUCCEEDED(hr))
    {
        // Extract fresh reference pointers and rebuild target binds.
        ID3D11Texture2D* pBackBuffer = nullptr;
        hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
        if (SUCCEEDED(hr))
        {
            hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
            pBackBuffer->Release();
        }
    }

    if (FAILED(hr))
        return 0;

    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    // Update the viewport dimensions.
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(width);
    vp.Height = static_cast<FLOAT>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);

    // Update projection matrix to match new aspect ratio.
    XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / static_cast<FLOAT>(height), 0.01f, 100.0f);
    XMStoreFloat4x4(&m_projection, proj);

    return 0;
}

// Called when the windoe is repainted.
LRESULT CDXView::OnPaint(UINT, WPARAM, LPARAM)
{
    Render();
    ValidateRect();
    return 0;
}

// Draw the cube in the window.
void CDXView::Render()
{
    if (!m_pImmediateContext || !m_pRenderTargetView)
        return;

    // Clear the back buffer frame view.
    float ClearColor[] = { 0.0f, 0.125f, 0.5f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    // Calculate the time based rotation.
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    float deltaTime = static_cast<float>(currentTime.QuadPart - m_lastTime.QuadPart) /
        static_cast<float>(m_frequency.QuadPart);
    m_lastTime = currentTime;

    // Rotate at 1.5 radian per second.
    m_rotationAngle += deltaTime * 1.5f;
    XMStoreFloat4x4(&m_world, XMMatrixRotationY(m_rotationAngle));

    // Package constant buffer parameters using matrix transposition registers.
    ConstantBuffer cb;
    cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_world));
    cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_view));
    cb.mProjection = XMMatrixTranspose(XMLoadFloat4x4(&m_projection));

    m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    // Set shaders, layouts, and constants into state context pipeline.
    m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
    m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);

    // Draw geometry call.
    m_pImmediateContext->DrawIndexed(36, 0, 0);

    // Present rendering frame back buffer loop swap.
    m_pSwapChain->Present(1, 0);
}

// The window procedure handles the window messages.
LRESULT CDXView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:   return OnSize(msg, wparam, lparam);
        case WM_PAINT:  return OnPaint(msg, wparam, lparam);
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
