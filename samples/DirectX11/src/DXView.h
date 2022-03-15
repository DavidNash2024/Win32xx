/////////////////////////////
// DXView.h
//

#ifndef DXVIEW_H
#define DXVIEW_H

#include "wxx_wincore.h"

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include "resource.h"

using namespace DirectX;

////////////
// Structures
//
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


////////////////////////////////////////////
// CView manages main window. It renders the
// DirectX11 images.
class CDXView : public CWnd
{
public:
    CDXView();
    virtual ~CDXView();
    void Render();

protected:
    // Virtual functions that override base class functions
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void CleanupDevice();
    HRESULT InitDevice();

    // Member variables
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
    XMMATRIX                m_world;
    XMMATRIX                m_view;
    XMMATRIX                m_projection;
};

#endif  // DXVIEW_H
