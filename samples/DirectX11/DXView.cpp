
#include "DXView.h"




//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompileFromFile( szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob );
    if( FAILED(hr) )
    {
        if( pErrorBlob )
        {
            OutputDebugStringA( reinterpret_cast<const char*>( pErrorBlob->GetBufferPointer() ) );
            pErrorBlob->Release();
        }
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------------------------
CDXView::~CDXView()
{
	CleanupDevice();
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT CDXView::InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    ::GetClientRect( m_hWnd, &rc );
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
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice( nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                D3D11_SDK_VERSION, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext );

        if ( hr == E_INVALIDARG )
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice( nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                                    D3D11_SDK_VERSION, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext );
        }

        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = m_pd3dDevice->QueryInterface( __uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice) );
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent( __uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory) );
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    // Create swap chain
    IDXGIFactory2* dxgiFactory2 = nullptr;
    hr = dxgiFactory->QueryInterface( __uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2) );
    if ( dxgiFactory2 )
    {
        // DirectX 11.1 or later
        hr = m_pd3dDevice->QueryInterface( __uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pd3dDevice1) );
        if (SUCCEEDED(hr))
        {
            (void) m_pImmediateContext->QueryInterface( __uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pImmediateContext1) );
        }

        DXGI_SWAP_CHAIN_DESC1 sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.Width = width;
        sd.Height = height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd( m_pd3dDevice, m_hWnd, &sd, nullptr, nullptr, &m_pSwapChain1 );
        if (SUCCEEDED(hr))
        {
            hr = m_pSwapChain1->QueryInterface( __uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain) );
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = m_hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain( m_pd3dDevice, &sd, &m_pSwapChain );
    }

    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation( m_hWnd, DXGI_MWA_NO_ALT_ENTER );

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
        return hr;

    hr = m_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &m_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, nullptr );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports( 1, &vp );

    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile( L"DirectX11.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        ::MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the vertex shader
	hr = m_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return hr;
	}

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
	hr = m_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &m_pVertexLayout );
	pVSBlob->Release();
	if( FAILED( hr ) )
        return hr;

    // Set the input layout
    m_pImmediateContext->IASetInputLayout( m_pVertexLayout );

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile( L"DirectX11.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        ::MessageBox( nullptr,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the pixel shader
	hr = m_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) },
    };
    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = vertices;
    hr = m_pd3dDevice->CreateBuffer( &bd, &InitData, &m_pVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

    // Create index buffer
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        0,5,4,
        1,5,0,

        3,4,7,
        0,4,3,

        1,6,5,
        2,6,1,

        2,7,6,
        3,7,2,

        6,4,5,
        7,4,6,
    };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( WORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
    InitData.pSysMem = indices;
    hr = m_pd3dDevice->CreateBuffer( &bd, &InitData, &m_pIndexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set index buffer
    m_pImmediateContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

    // Set primitive topology
    m_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = m_pd3dDevice->CreateBuffer( &bd, nullptr, &m_pConstantBuffer );
    if( FAILED( hr ) )
        return hr;

    // Initialize the world matrix
	m_World = XMMatrixIdentity();

    // Initialize the view matrix
	XMVECTOR Eye = XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	m_View = XMMatrixLookAtLH( Eye, At, Up );

    // Initialize the projection matrix
	m_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CDXView::CleanupDevice()
{
    if( m_pImmediateContext ) m_pImmediateContext->ClearState();

    if( m_pConstantBuffer ) m_pConstantBuffer->Release();
    if( m_pVertexBuffer ) m_pVertexBuffer->Release();
    if( m_pIndexBuffer ) m_pIndexBuffer->Release();
    if( m_pVertexLayout ) m_pVertexLayout->Release();
    if( m_pVertexShader ) m_pVertexShader->Release();
    if( m_pPixelShader ) m_pPixelShader->Release();
    if( m_pRenderTargetView ) m_pRenderTargetView->Release();
    if( m_pSwapChain1 ) m_pSwapChain1->Release();
    if( m_pSwapChain ) m_pSwapChain->Release();
    if( m_pImmediateContext1 ) m_pImmediateContext1->Release();
    if( m_pImmediateContext ) m_pImmediateContext->Release();
    if( m_pd3dDevice1 ) m_pd3dDevice1->Release();
    if( m_pd3dDevice ) m_pd3dDevice->Release();
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void CDXView::Render()
{
    // Update our time
    static float t = 0.0f;
    if( m_driverType == D3D_DRIVER_TYPE_REFERENCE )
    {
        t += ( float )XM_PI * 0.0125f;
    }
    else
    {
        static ULONGLONG timeStart = 0;
        ULONGLONG timeCur = GetTickCount64();
        if( timeStart == 0 )
            timeStart = timeCur;
        t = ( timeCur - timeStart ) / 1000.0f;
    }

    //
    // Animate the cube
    //
	m_World = XMMatrixRotationY( t );

    //
    // Clear the back buffer
    //
    m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, Colors::MidnightBlue );

    //
    // Update variables
    //
    ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose( m_World );
	cb.mView = XMMatrixTranspose( m_View );
	cb.mProjection = XMMatrixTranspose( m_Projection );
	m_pImmediateContext->UpdateSubresource( m_pConstantBuffer, 0, nullptr, &cb, 0, 0 );

    //
    // Renders a triangle
    //
	m_pImmediateContext->VSSetShader( m_pVertexShader, nullptr, 0 );
	m_pImmediateContext->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
	m_pImmediateContext->PSSetShader( m_pPixelShader, nullptr, 0 );
	m_pImmediateContext->DrawIndexed( 36, 0, 0 );        // 36 vertices needed for 12 triangles in a triangle list

    //
    // Present our back buffer to our front buffer
    //
    m_pSwapChain->Present( 0, 0 );
}

