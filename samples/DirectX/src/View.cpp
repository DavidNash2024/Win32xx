//////////////////////////////////////////////
// View.cpp

// Includes backbuffer resizing suggested by Guillaume Werlé

// NOTE: for MS compilers you will need the DirectX SDK v9
//       for Dev-C++ you will need the DirecX v9.0c DevPak


#include "stdafx.h"
#include <d3dx9.h>	// see the note above
#include "DXApp.h"


/////////////////////////////////////////////////
// Definitions for the CDXView class
//
BOOL CDXView::CDXThread::InitInstance()
{
	// This function runs when the thread starts

	CMainFrame& Frame = GetDXApp().GetMainFrame();
	CDXView& DXView = Frame.GetDXView();
	CDX& DX = DXView.GetDX();
	
	// assign the m_pDX member variable
	m_pDX = &DX;

	// Create the DX window
	DX.Create(DXView);

	return TRUE;	// return TRUE to run the message loop
}

int CDXView::CDXThread::MessageLoop()
// Here we override CWinThread::MessageLoop to accommodate the special needs of DirectX
{
	MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));
	while( Msg.message != WM_QUIT )
	{
		if ( PeekMessage(&Msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&Msg);
			::DispatchMessage(&Msg);
		}
		else
		{
			// Force thread to yield
			if (WAIT_TIMEOUT == ::WaitForSingleObject(*this, 1))
				m_pDX->Render();
		}
	}

	return LOWORD(Msg.wParam);
} 

/////////////////////////////////////////////////
// Definitions for the CDXView class
//
CDXView::CDX::CDX() : m_pD3D(NULL), m_pd3dDevice(NULL), m_pVB(NULL)
{
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

HRESULT CDXView::CDX::InitD3D( HWND hWnd )
// Initializes Direct3D
{
    // Create the D3D object.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
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
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                    //  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &m_d3dpp, &m_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

	SetupDefaultRenderStates();

    return S_OK;
}

HRESULT CDXView::CDX::InitGeometry()
// Creates the scene geometry.
{
    // Initialize three vertices for rendering a triangle
    CUSTOMVERTEX g_Vertices[] =
    {
        { -1.0f,-1.0f, 0.0f, 0xffff0000, },
        {  1.0f,-1.0f, 0.0f, 0xff0000ff, },
        {  0.0f, 1.0f, 0.0f, 0xffffffff, }
    };

    // Create the vertex buffer.
    if( FAILED( m_pd3dDevice->CreateVertexBuffer( 3*sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_XYZ | D3DFVF_DIFFUSE,
                                                  D3DPOOL_MANAGED, &m_pVB, NULL ) ) )
    {
        return E_FAIL;
    }

    // Fill the vertex buffer.
    VOID* pVertices;
    if( FAILED( m_pVB->Lock( 0, sizeof(g_Vertices), (void**)&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );
    m_pVB->Unlock();

    return S_OK;
}

int CDXView::CDX::OnCreate(CREATESTRUCT& cs)
{
	UNREFERENCED_PARAMETER(cs);

	// Initialize Direct3D
	if( SUCCEEDED( InitD3D( *this ) ) )
	{
		// Create the scene geometry
        if( SUCCEEDED( InitGeometry() ) )
        {
			// Show the window
			ShowWindow(SW_SHOWDEFAULT);
			UpdateWindow();
		}
	}
	else
		TRACE("Failed to initialize DirectX\n");
	
	return 0;
}

void CDXView::CDX::OnDestroy()
{
	// End this thread
	::PostQuitMessage(0);
}

LRESULT CDXView::CDX::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int cx = GET_X_LPARAM(lParam);
	int cy = GET_Y_LPARAM(lParam);
	
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_SHOWWINDOW);
	return FinalWindowProc(uMsg, wParam, lParam);
}

void CDXView::CDX::PreCreate(CREATESTRUCT &cs)
{
	// An initial window size to allow InitD3D to succeed
	cs.x = 0;
	cs.y = 0;
	cs.cx = 100;
	cs.cy = 100;
}

void CDXView::CDX::SetupMatrices()
// Sets up the world, view, and projection transform Matrices.
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

void CDXView::CDX::Render()
// Draws the scene.
{
	if (IsWindow() && m_pd3dDevice)
	{
		HRESULT hResult = m_pd3dDevice->TestCooperativeLevel();
		switch (hResult)
		{
		case D3D_OK:
			{
				CRect rcClient = GetClientRect();
				bool bNeedResize = (int)m_d3dpp.BackBufferWidth != rcClient.Width() || (int)m_d3dpp.BackBufferHeight != rcClient.Height();
				if (bNeedResize)
				{
					m_d3dpp.BackBufferWidth		= rcClient.Width();
					m_d3dpp.BackBufferHeight	= rcClient.Height();
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
				m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
			}
			break;
		case D3DERR_DEVICELOST:
			TRACE("Got D3DERR_DEVICELOST\n");
			break;
		case D3DERR_DEVICENOTRESET:
			TRACE("Got D3DERR_DEVICENOTRESET\n");
			m_pd3dDevice->Reset(&m_d3dpp);	// Reset the DX device
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

LRESULT CDXView::CDX::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case UWM_RESIZE: return OnSize(uMsg, wParam, lParam);
	}

	return WndProcDefault(uMsg, wParam, lParam);
}


/////////////////////////////////////////////////
// Definitions for the CDXView class
//
CDXView::~CDXView()
{
	// Ensure the DXThread ends before destroying this object.
	::WaitForSingleObject(m_DXThread.GetThread(), INFINITE);
}

int CDXView::OnCreate(CREATESTRUCT& cs)
{
	// Create our thread. The thread creates the DX child window when starts
	m_DXThread.CreateThread();
	
	return CWnd::OnCreate(cs);
}

LRESULT CDXView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_DX.IsWindow())
		m_DX.PostMessage(UWM_RESIZE, wParam, lParam);

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CDXView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_SIZE: return OnSize(uMsg, wParam, lParam);
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

