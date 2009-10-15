//////////////////////////////////////////////
// View.cpp

// NOTE: for MS compilers you will need the DirectX SDK v9
//       for Dev-C++ you will need the DirecX v9.0c DevPak

#include "../../Win32++/wincore.h"
#include <d3dx9.h>	// see the note above
#include <mmsystem.h>
#include "View.h"
#include "Resource.h"

CView::~CView()
{
	if(m_pVB != NULL)
		m_pVB->Release();

	if( m_pd3dDevice != NULL)
        m_pd3dDevice->Release();

    if( m_pD3D != NULL)
        m_pD3D->Release();
}

void CView::OnCreate()
{
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// Initialize Direct3D
	if( SUCCEEDED( InitD3D( m_hWnd ) ) )
	{
		// Create the scene geometry
        if( SUCCEEDED( InitGeometry() ) )
        {
			// Show the window
			ShowWindow(SW_SHOWDEFAULT);
			UpdateWindow();
		}
	}
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	cs.x = 50;
	cs.y = 50;
	cs.cx = 400;
	cs.cy = 400;
	cs.lpszName = LoadString(IDW_MAIN);
}

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT CView::InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the D3DDevice
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                    //  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling, so we see the front and back of the triangle
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting, since we are providing our own vertex colors
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Creates the scene geometry
//-----------------------------------------------------------------------------
HRESULT CView::InitGeometry()
{
    // Initialize three vertices for rendering a triangle
    CUSTOMVERTEX g_Vertices[] =
    {
        { -1.0f,-1.0f, 0.0f, 0xffff0000, },
        {  1.0f,-1.0f, 0.0f, 0xff0000ff, },
        {  0.0f, 1.0f, 0.0f, 0xffffffff, },
    };

    // Create the vertex buffer.
    if( FAILED( m_pd3dDevice->CreateVertexBuffer( 3*sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_XYZ | D3DFVF_DIFFUSE,
                                                  D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
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

//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform Matrices.
//-----------------------------------------------------------------------------
VOID CView::SetupMatrices()
{
    // For our world matrix, we will just rotate the object about the y-axis.
    D3DXMATRIXA16 matWorld;

    // Set up the rotation matrix to generate 1 full rotation (2*PI radians)
    // every 1000 ms. To avoid the loss of precision inherent in very high
    // floating point numbers, the system time is modulated by the rotation
    // period before conversion to a radian angle.
    UINT  iTime  = timeGetTime() % 1000;

    FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
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



//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID CView::Render()
{
    // Clear the backbuffer to a black color
    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {
        // Setup the world, view, and projection Matrices
        SetupMatrices();

        // Render the vertex buffer contents
        m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(CUSTOMVERTEX) );
        m_pd3dDevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
        m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );

        // End the scene
        m_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}


LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		::PostQuitMessage( 0 );
		break;

	case WM_SIZING:
		Render();
		break;

	case WM_PAINT:
		Render();
		::ValidateRect( m_hWnd, NULL );
		break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
}


