/////////////////////////////
// DXView.h
//

#pragma once

#include "wxx_wincore.h"               // The core part of the Win32++ library.
#include <wrl/client.h>                // Provides the ComPtr smart pointer.
#include <wrl/wrappers/corewrappers.h> // RAII smart wrappers for Win32 system objects.
#include <d3d12.h>                     // Core DirectX 12 API header containing the device.
#include <dxgi1_4.h>                   // DirectX Graphics Infrastructure API version 1.4.

//////////////////////////////////////
// CDXView manages the DirectX window.
class CDXView : public CWnd
{
public:
    CDXView() = default;
    virtual ~CDXView() override = default;

    bool CreatePipelineState();
    bool InitDevice();
    void Render();
    void ResizeBuffers(int width, int height);

protected:
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnDestroy() override;
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT OnSizing(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    // Window values
    int m_width = 0;
    int m_height = 0;
    int m_triangleAngle = 0;

    // Core DX12 Objects
    Microsoft::WRL::ComPtr<ID3D12Device> m_device = nullptr;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue = nullptr;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator = nullptr;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList = nullptr;

    // Swap chain & Render Targets (Converted to ComPtr)
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain = nullptr;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[2] = {};
    UINT m_rtvIncrementSize = 0;

    // Pipeline & Resources (Converted to ComPtr)
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature = nullptr;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState = nullptr;

    // Synchronization
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence = nullptr;
    UINT64 m_fenceValue = 0;
    Microsoft::WRL::Wrappers::Event m_fenceEvent;
};

// Helper function declarations
void set_blend_state(D3D12_BLEND_DESC& blend_desc);
void set_rasterizer_state(D3D12_RASTERIZER_DESC& rasterizer_desc);
void set_depth_stencil_state(D3D12_DEPTH_STENCIL_DESC& depth_stencil_desc);
