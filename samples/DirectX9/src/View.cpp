/////////////////////////////
// View.cpp
//

// Includes backbuffer resizing suggested by Guillaume Werlé

// NOTE: for MS compilers you will need the DirectX SDK v9
//       for Dev-C++ you will need the DirecX v9.0c DevPak


#include "stdafx.h"
#include <d3dx9.h>  // see the note above
#include "DXApp.h"


///////////////////////////////
// CDXView function definitions
//

// This function runs when the thread starts
BOOL CDXView::CDXThread::InitInstance()
{
    CMainFrame& frame = GetDXApp()->GetMainFrame();
    const CDXView& dxView = frame.GetDXView();
    CDX& DX = dxView.GetDX();

    // assign the m_pDX member variable
    m_pDX = &DX;

    // Create the DX window
    DX.Create(dxView);

    return TRUE;    // return TRUE to run the message loop
}

// Here we override CWinThread::MessageLoop to accommodate the special needs of DirectX
int CDXView::CDXThread::MessageLoop()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while( msg.message != WM_QUIT )
    {
        if ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            // Force thread to yield
            if (WAIT_TIMEOUT == ::WaitForSingleObject(*this, 1))
                m_pDX->Render();
        }
    }

    return LOWORD(msg.wParam);
}

///////////////////////////////
// CDXView function definitions
//
CDXView::CDX::CDX() : m_pD3D(NULL), m_pd3dDevice(NULL), m_pVB(NULL)
{
    ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
}

CDXView::CDX::~CDX()
{
    if(m_pVB != NULL)
        m_pVB->Release();

    if( m_pd3dDevice != NULL)
        m_pd3dDevice->Release();

    if( m_pD3D != NULL)
        m_pD3D->Release();
}

// Initializes Direct3D
HRESULT CDXView::CDX::InitD3D( HWND hWnd )
{
    // Create the D3D object.
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (m_pD3D == NULL)
        return E_FAIL;

    CRect rc = GetClientRect();

    // Set up the structure used to create the D3DDevice
    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.Windowed = TRUE;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    m_d3dpp.BackBufferWidth = rc.Width();
    m_d3dpp.BackBufferHeight = rc.Height();

    // Create the D3DDevice
    if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
               D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice)))
    {
        // Attempt CreateDevice with different options.
        if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
                   D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3dpp, &m_pd3dDevice)))
        {
            return E_FAIL;
        }
    }

    SetupDefaultRenderStates();

    return S_OK;
}

// Creates the scene geometry.
HRESULT CDXView::CDX::InitGeometry()
{
    // Initialize three vertices for rendering a triangle
    CUSTOMVERTEX vertices[] =
    {
        { -1.0f,-1.0f, 0.0f, 0xffff0000, },
        {  1.0f,-1.0f, 0.0f, 0xff0000ff, },
        {  0.0f, 1.0f, 0.0f, 0xffffffff, }
    };

    // Create the vertex buffer.
    if (FAILED(m_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
                                                0, D3DFVF_XYZ | D3DFVF_DIFFUSE,
                                                D3DPOOL_MANAGED, &m_pVB, NULL)))
    {
        return E_FAIL;
    }

    // Fill the vertex buffer.
    VOID* pVertices;
    if (FAILED(m_pVB->Lock(0, sizeof(vertices), &pVertices, 0)))
        return E_FAIL;
    memcpy(pVertices, vertices, sizeof(vertices));
    m_pVB->Unlock();

    return S_OK;
}

// Called when the window is created.
int CDXView::CDX::OnCreate(CREATESTRUCT&)
{
    // Initialize Direct3D
    if (SUCCEEDED(InitD3D(*this)))
    {
        // Create the scene geometry
        if (SUCCEEDED(InitGeometry()))
        {
            // Resize this window to fill the parent window.
            CRect rc = GetParent().GetClientRect();
            SetWindowPos(0, 0, 0, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

            // Show the window
            ShowWindow(SW_SHOWDEFAULT);
            UpdateWindow();
        }
    }
    else
        TRACE("Failed to initialize DirectX\n");

    return 0;
}

// Called when the window is resized.
LRESULT CDXView::CDX::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    int cx = GET_X_LPARAM(lparam);
    int cy = GET_Y_LPARAM(lparam);

    SetWindowPos(0, 0, 0, cx, cy, SWP_SHOWWINDOW);
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the window is created. Sets the window creation parameters.
void CDXView::CDX::PreCreate(CREATESTRUCT& cs)
{
    // An initial window size to allow InitD3D to succeed
    cs.x = 0;
    cs.y = 0;
    cs.cx = 100;
    cs.cy = 100;
}

// Sets up the world, view, and projection transform Matrices.
void CDXView::CDX::SetupMatrices()
{
    // For our world matrix, we will just rotate the object about the y-axis.
    D3DXMATRIXA16 matWorld;

    // Set up the rotation matrix to generate 1 full rotation (2*PI radians)
    // every 2000 ms. To avoid the loss of precision inherent in very high
    // floating point numbers, the system time is modulated by the rotation
    // period before conversion to a radian angle.
    UINT  iTime  = timeGetTime() % 2000;

    FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 2000.0f;
    D3DXMatrixRotationY( &matWorld, fAngle );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-5.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

// Draws the scene.
void CDXView::CDX::Render()
{
    if (IsWindow() && m_pd3dDevice)
    {
        HRESULT hResult = m_pd3dDevice->TestCooperativeLevel();
        switch (hResult)
        {
        case D3D_OK:
            {
                CRect rcClient = GetClientRect();
                int backBufferWidth = static_cast<int>(m_d3dpp.BackBufferWidth);
                int backBufferHeight = static_cast<int>(m_d3dpp.BackBufferHeight);
                bool isResizeNeeded = backBufferWidth != rcClient.Width() || backBufferHeight != rcClient.Height();
                if (isResizeNeeded)
                {
                    m_d3dpp.BackBufferWidth     = rcClient.Width();
                    m_d3dpp.BackBufferHeight    = rcClient.Height();
                    if ( !SUCCEEDED( m_pd3dDevice->Reset(&m_d3dpp) ) )
                        TRACE("Failed to reset the DirectX device\n");
                }

                // Clear the backbuffer to a black color
                if (D3D_OK !=m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 ))
                    TRACE("Failed to clear back buffer\n");

                // Begin the scene
                if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
                {
                    // Setup the world, view, and projection Matrices
                    SetupMatrices();

                    SetupDefaultRenderStates();

                    // Render the vertex buffer contents
                    m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
                    m_pd3dDevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
                    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );

                    // End the scene
                    m_pd3dDevice->EndScene();
                }
                else
                    TRACE("Failed to render the scene\n");

                // Present the backbuffer contents to the display
                m_pd3dDevice->Present( NULL, NULL, 0, NULL );
            }
            break;
        case D3DERR_DEVICELOST:
            TRACE("Got D3DERR_DEVICELOST\n");
            break;
        case D3DERR_DEVICENOTRESET:
            TRACE("Got D3DERR_DEVICENOTRESET\n");
            m_pd3dDevice->Reset(&m_d3dpp);  // Reset the DX device
            break;
        default:
            TRACE("Direct3D device is in an invalid state\n");
            break;
        }
    }
}

void CDXView::CDX::SetupDefaultRenderStates()
{
    // Turn off culling, so we see the front and back of the triangle
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting, since we are providing our own vertex colors
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
}

// Process the window's messages.
LRESULT CDXView::CDX::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
        case UWM_RESIZE: return OnSize(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}


///////////////////////////////
// CDXView function definitions
//

// Destructor.
CDXView::~CDXView()
{
    // Ensure the DXThread ends before destroying this object.
    ::WaitForSingleObject(m_dxThread.GetThread(), INFINITE);
}

// Called when the window is created.
int CDXView::OnCreate(CREATESTRUCT& cs)
{
    // Create our thread. The thread creates the DX child window when starts
    m_dxThread.CreateThread();

    return CWnd::OnCreate(cs);
}

// Called when the window is destroyed.
void CDXView::OnDestroy()
{
    m_dxThread.PostThreadMessage(WM_QUIT, 0, 0);
    ::WaitForSingleObject(m_dxThread.GetThread(), INFINITE);
}

// Called when the window is resized.
LRESULT CDXView::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (m_dx.IsWindow())
        m_dx.PostMessage(UWM_RESIZE, wparam, lparam);

    return FinalWindowProc(msg, wparam, lparam);
}

// Process the window's messages.
LRESULT CDXView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE: return OnSize(msg, wparam, lparam);
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

