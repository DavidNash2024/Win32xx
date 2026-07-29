/////////////////////////////
// DXView.h
//

#pragma once

#include "wxx_wincore.h"
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>

// Direct3D libraries required for linking
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

/////////////
// Structures
//
struct SimpleVertex
{
    DirectX::XMFLOAT3 Pos;
    DirectX::XMFLOAT4 Color;
};

struct ConstantBuffer
{
    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProjection;
};

////////////////////////////////////////////////////////////////
// CDXView manages main window. It renders the DirectX11 images.
class CDXView : public CWnd
{
public:
    CDXView() = default;
    virtual ~CDXView() override = default;

    HRESULT CreateD3DDevice();
    HRESULT CreateSwapChain(Microsoft::WRL::ComPtr<IDXGIFactory1>& factory, UINT width, UINT height);
    HRESULT CreateRenderTarget(UINT width, UINT height);
    HRESULT GetDXGIFactory(Microsoft::WRL::ComPtr<IDXGIFactory1>& factory);
    HRESULT InitDevice();
    HRESULT InitShaders();
    HRESULT InitGeometry();
    void    InitCamera(UINT width, UINT height);
    void    Render();
    HRESULT ResizeBuffers(UINT width, UINT height);

protected:
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnDestroy() override;
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDXView(const CDXView&) = delete;
    CDXView& operator=(const CDXView&) = delete;

    // Message handlers called by WndProc
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSizing(UINT msg, WPARAM wParam, LPARAM lParam);

    // Member variables
    Microsoft::WRL::ComPtr<ID3D11Device>           m_pd3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>          m_pd3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_pImmediateContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>   m_pImmediateContext1;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         m_pSwapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>        m_pSwapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>     m_pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>      m_pPixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>      m_pVertexLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_pIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>           m_pConstantBuffer;

    D3D_DRIVER_TYPE   m_driverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
    DirectX::XMFLOAT4X4 m_world{ 1.0f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 1.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 1.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f, 1.0f };

    DirectX::XMFLOAT4X4 m_view = m_world;
    DirectX::XMFLOAT4X4 m_projection = m_world;

    LARGE_INTEGER m_frequency = {};
    LARGE_INTEGER m_lastTime = {};
    float m_rotationAngle = 0.0f;
};
