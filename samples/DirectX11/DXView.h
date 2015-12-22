
#ifndef DXVIEW_H
#define DXVIEW_H

#include "wxx_wincore.h"

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>



#include "resource.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};



class CDXView : public CWnd
{
public:
	~CDXView();
	void CleanupDevice();
	HRESULT InitDevice();
	void Render();

protected:
	int OnCreate(CREATESTRUCT& cs)
	{
		// OnCreate is called automatically during window creation when a
		// WM_CREATE message received.

		// Tasks such as setting the icon, creating child windows, or anything
		// associated with creating windows are normally performed here.

		UNREFERENCED_PARAMETER(cs);

		// Set the window's icon
		SetIconSmall(IDI_DIRECTX11);
		SetIconLarge(IDI_DIRECTX11);

		// Set the window title
		SetWindowText(LoadString(IDS_APP_TITLE));

		InitDevice();

		TRACE("OnCreate\n");
		return 0;
	}

	virtual void PreCreate(CREATESTRUCT& cs)
	{
		cs.cx = 800;
		cs.cy = 600;
	}

	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (uMsg)
		{
		case WM_PAINT:
			hdc = BeginPaint(ps);
			EndPaint(ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

			// Note that this tutorial does not handle resizing (WM_SIZE) requests,
			// so we created the window without the resize border.
		}

		return WndProcDefault(uMsg, wParam, lParam);
	}

private:
	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pd3dDevice = nullptr;
	ID3D11Device1*          m_pd3dDevice1 = nullptr;
	ID3D11DeviceContext*    m_pImmediateContext = nullptr;
	ID3D11DeviceContext1*   m_pImmediateContext1 = nullptr;
	IDXGISwapChain*         m_pSwapChain = nullptr;
	IDXGISwapChain1*        m_pSwapChain1 = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11VertexShader*     m_pVertexShader = nullptr;
	ID3D11PixelShader*      m_pPixelShader = nullptr;
	ID3D11InputLayout*      m_pVertexLayout = nullptr;
	ID3D11Buffer*           m_pVertexBuffer = nullptr;
	ID3D11Buffer*           m_pIndexBuffer = nullptr;
	ID3D11Buffer*           m_pConstantBuffer = nullptr;
	XMMATRIX                m_World;
	XMMATRIX                m_View;
	XMMATRIX                m_Projection;

};

#endif	// DXVIEW_H