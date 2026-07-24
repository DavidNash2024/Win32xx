/////////////////////////////
// DXView.h
//


#pragma once

#include "wxx_wincore.h"
#include <d3d12.h>
#include <dxgi1_4.h>


class CDXView : public CWnd
{
public:
    CDXView() = default;
    virtual ~CDXView() override { CleanupDirectX(); }

    void CleanupDirectX();
    bool CreatePipelineState();
    bool InitDevice();
    void Render();

protected:
    virtual int OnCreate(CREATESTRUCT& cs) override;
    virtual void OnDestroy() override { ::PostQuitMessage(0); }
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    // Window values
    int m_width = 800;
    int m_height = 800;
    int m_triangleAngle = 0;

    // Core DX12 Objects
    ID3D12Device* m_device = nullptr;
    ID3D12CommandQueue* m_commandQueue = nullptr;
    ID3D12CommandAllocator* m_commandAllocator = nullptr;
    ID3D12GraphicsCommandList* m_commandList = nullptr;

    // Swap chain & Render Targets
    IDXGISwapChain3* m_swapChain = nullptr;
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;
    ID3D12Resource* m_renderTargets[2] = {};
    UINT m_rtvIncrementSize = 0;

    // Pipeline & Resources
    ID3D12RootSignature* m_rootSignature = nullptr;
    ID3D12PipelineState* m_pipelineState = nullptr;

    // Synchronization
    ID3D12Fence* m_fence = nullptr;
    UINT64 m_fenceValue = 0;
    HANDLE m_fenceEvent = nullptr;
};


void set_blend_state(D3D12_BLEND_DESC& blend_desc);
void set_rasterizer_state(D3D12_RASTERIZER_DESC& rasterizer_desc);
void set_depth_stencil_state(D3D12_DEPTH_STENCIL_DESC &depth_stencil_desc);
