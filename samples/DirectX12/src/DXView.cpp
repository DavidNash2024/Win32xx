/////////////////////////////
// DXView.cpp
//


#include "wxx_wincore.h"
#include <d3dcompiler.h>
#include "DXView.h"
#include "resource.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

////////////////////////////////
// CDXView function definitions.
//

void CDXView::CleanupDirectX()
{
    if (m_fenceEvent) CloseHandle(m_fenceEvent);
    if (m_pipelineState) m_pipelineState->Release();
    if (m_rootSignature) m_rootSignature->Release();
    if (m_commandList) m_commandList->Release();
    if (m_commandAllocator) m_commandAllocator->Release();
    if (m_commandQueue) m_commandQueue->Release();
    if (m_rtvHeap) m_rtvHeap->Release();
    for (int i = 0; i < 2; i++)
    {
        if (m_renderTargets[i]) m_renderTargets[i]->Release();
    }

    if (m_swapChain) m_swapChain->Release();
    if (m_device) m_device->Release();
}

bool CDXView::CreatePipelineState()
{
    // Define and configure the Root Signature parameters.
    D3D12_ROOT_PARAMETER root_parameters[1] = {};
    root_parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    root_parameters[0].Constants.Num32BitValues = 1;
    root_parameters[0].Constants.ShaderRegister = 0;
    root_parameters[0].Constants.RegisterSpace = 0;
    root_parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

    D3D12_ROOT_SIGNATURE_DESC root_signature_desc = {};
    root_signature_desc.NumParameters = _countof(root_parameters);
    root_signature_desc.pParameters = root_parameters;
    root_signature_desc.NumStaticSamplers = 0;
    root_signature_desc.pStaticSamplers = nullptr;
    root_signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // Serialize and create the Root Signature.
    ID3DBlob* signature_blob = nullptr;
    ID3DBlob* error_blob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature_blob, &error_blob);

    if (FAILED(hr))
    {
        if (error_blob)
        {
            OutputDebugStringA(static_cast<const char*>(error_blob->GetBufferPointer()));
            error_blob->Release();
        }

        ::MessageBox(nullptr, L"Root signature serialization failed.", L"Error", MB_OK);
        return false;
    }

    if (signature_blob)
    {
        hr = m_device->CreateRootSignature(0, signature_blob->GetBufferPointer(), signature_blob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
        signature_blob->Release();
        if (FAILED(hr)) return false;
    }

    // Compile the Vertex Shader.
    ID3DBlob* vertex_shader = nullptr;
    error_blob = nullptr;
    hr = D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vertex_shader, &error_blob);
    if (FAILED(hr))
    {
        if (error_blob)
        {
            OutputDebugStringA(static_cast<const char*>(error_blob->GetBufferPointer()));
            error_blob->Release();
        }

        ::MessageBox(nullptr, L"Vertex shader compilation failed.", L"Shader Error", MB_OK);
        return false;
    }
    if (error_blob) { error_blob->Release(); error_blob = nullptr; } // Clean up warning blobs

    // Compile the Pixel Shader.
    ID3DBlob* pixel_shader = nullptr;
    hr = D3DCompileFromFile(L"shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &pixel_shader, &error_blob);
    if (FAILED(hr))
    {
        if (error_blob)
        {
            OutputDebugStringA(static_cast<const char*>(error_blob->GetBufferPointer()));
            error_blob->Release();
        }

        ::MessageBox(nullptr, L"Pixel shader compilation failed.", L"Shader Error", MB_OK);
        if (vertex_shader) vertex_shader->Release();
        return false;
    }
    if (error_blob) { error_blob->Release(); error_blob = nullptr; } // Clean up warning blobs

    // Populate and configure the Pipeline State Description (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
    pso_desc.pRootSignature = m_rootSignature;
    pso_desc.VS.pShaderBytecode = vertex_shader->GetBufferPointer();
    pso_desc.VS.BytecodeLength = vertex_shader->GetBufferSize();
    pso_desc.PS.pShaderBytecode = pixel_shader->GetBufferPointer();
    pso_desc.PS.BytecodeLength = pixel_shader->GetBufferSize();

    set_blend_state(pso_desc.BlendState);
    pso_desc.SampleMask = UINT_MAX;
    set_rasterizer_state(pso_desc.RasterizerState);
    set_depth_stencil_state(pso_desc.DepthStencilState);

    pso_desc.InputLayout.pInputElementDescs = nullptr;
    pso_desc.InputLayout.NumElements = 0;
    pso_desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso_desc.NumRenderTargets = 1;
    pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pso_desc.SampleDesc.Count = 1;
    pso_desc.SampleDesc.Quality = 0;

    // Create the compiled Pipeline State Object.
    hr = m_device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_pipelineState));
    vertex_shader->Release();
    pixel_shader->Release();

    if (FAILED(hr))
    {
        ::MessageBox(nullptr, L"Pipeline State Object creation failed.", L"PSO Error", MB_OK);
        return false;
    }

    return true;
}

bool CDXView::InitDevice()
{
    // Create the core virtual device representation of the GPU.
    HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
    if (FAILED(hr)) return false;

    // Create the command queue.
    D3D12_COMMAND_QUEUE_DESC queue_desc = {};
    queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    hr = m_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&m_commandQueue));
    if (FAILED(hr)) return false;

    // Create the command allocator.
    hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
    if (FAILED(hr)) return false;

    hr = m_commandAllocator->Reset();
    if (FAILED(hr)) return false;

    // Create the recording command list.
    hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList));
    if (FAILED(hr)) return false;

    hr = m_commandList->Close();
    if (FAILED(hr)) return false;

    // Create DXGI Factory to handle swap chain creation.
    IDXGIFactory4* factory = nullptr;
    hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr)) return false;

    // Configure and create the Swap Chain.
    DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
    swap_chain_desc.BufferCount = 2;
    swap_chain_desc.BufferDesc.Width = m_width;
    swap_chain_desc.BufferDesc.Height = m_height;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swap_chain_desc.OutputWindow = *this;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.Windowed = TRUE;

    IDXGISwapChain* temp_swap_chain = nullptr;
    hr = factory->CreateSwapChain(m_commandQueue, &swap_chain_desc, &temp_swap_chain);

    // The factory infrastructure is no longer needed after this point.
    factory->Release();
    if (FAILED(hr)) return false;

    // Upcast the swap chain to version 3 to leverage modern back buffer indexing.
    hr = temp_swap_chain->QueryInterface(IID_PPV_ARGS(&m_swapChain));
    temp_swap_chain->Release();
    if (FAILED(hr)) return false;

    // Allocate a descriptor heap to hold Render Target Views (RTV).
    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
    rtv_heap_desc.NumDescriptors = 2;
    rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    hr = m_device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&m_rtvHeap));
    if (FAILED(hr)) return false;

    // Retrieve and bind the back buffers to the RTV heap descriptor locations.
    m_rtvIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT i = 0; i < 2; i++)
    {
        hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
        if (FAILED(hr)) return false;

        m_device->CreateRenderTargetView(m_renderTargets[i], nullptr, rtv_handle);
        rtv_handle.ptr += m_rtvIncrementSize;
    }

    // Create the CPU/GPU fence synchronization objects.
    hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    if (FAILED(hr)) return false;

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!m_fenceEvent) return false;

    return true;
}

int CDXView::OnCreate(CREATESTRUCT&)
{
    // Set the window's icon
    SetIconSmall(IDI_DIRECTX12);
    SetIconLarge(IDI_DIRECTX12);

    // Set the window title
    SetWindowText(LoadString(IDS_APP_TITLE));

    if (!InitDevice())
    {
        CleanupDirectX();
        return -1;
    }

    if (!CreatePipelineState())
    {
        CleanupDirectX();
        MessageBox(L"Failed to Initialize", L"Error", MB_OK);
        return -1;
    }

    return 0;
}

// Specifiy the CREATESTRUCT parameters before creating the window.
void CDXView::PreCreate(CREATESTRUCT& cs)
{
    CRect windowRect(0, 0, m_width, m_height);
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    cs.cx = windowRect.Width();
    cs.cy = windowRect.Height();
}

void CDXView::Render()
{
    // Prepare command allocators and lists for recording.
    m_commandAllocator->Reset();
    m_commandList->Reset(m_commandAllocator, m_pipelineState);

    // Identify current active swap chain buffer index.
    UINT backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    // Transition back buffer from PRESENT state to RENDER_TARGET state.
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_renderTargets[backBufferIndex];
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    m_commandList->ResourceBarrier(1, &barrier);

    // Calculate descriptor offset pointer for the current back buffer RTV.
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += backBufferIndex * m_rtvIncrementSize;

    // Clear the active Render Target View.
    float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Establish Viewport and Scissor boundary states.
    D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, 1.0f };
    D3D12_RECT scissorRect = { 0, 0, m_width, m_height };
    m_commandList->RSSetViewports(1, &viewport);
    m_commandList->RSSetScissorRects(1, &scissorRect);

    // Bind pipeline states, root signatures, and output targets.
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_commandList->SetGraphicsRootSignature(m_rootSignature);
    m_commandList->SetPipelineState(m_pipelineState);

    // Push animation angle and issue the draw call.
    m_commandList->SetGraphicsRoot32BitConstant(0, m_triangleAngle, 0);
    m_commandList->DrawInstanced(3, 1, 0, 0);

    // Transition back buffer back from RENDER_TARGET to PRESENT state.
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_commandList->ResourceBarrier(1, &barrier);

    // Close command recording and dispatch to the GPU command queue.
    m_commandList->Close();
    ID3D12CommandList* commandLists[] = { m_commandList };
    m_commandQueue->ExecuteCommandLists(1, commandLists);

    // Present the finished frame to the display.
    m_swapChain->Present(1, 0);

    // CPU/GPU Fence Sync: Wait for the frame execution to finish on the GPU hardware.
    const UINT64 currentFenceValue = ++m_fenceValue;
    m_commandQueue->Signal(m_fence, currentFenceValue);

    if (m_fence->GetCompletedValue() < currentFenceValue)
    {
        m_fence->SetEventOnCompletion(currentFenceValue, m_fenceEvent);
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    // Increment the rotation angle for the next frame's animation.
    m_triangleAngle++;
}

LRESULT CDXView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // switch (msg)
    // {
    //     default: break;
    // }

    return WndProcDefault(msg, wparam, lparam);
}


////////////////////
// Global functions.
//

void set_blend_state(D3D12_BLEND_DESC& blend_desc)
{
    blend_desc = {};
    blend_desc.AlphaToCoverageEnable = FALSE;
    blend_desc.IndependentBlendEnable = FALSE;

    D3D12_RENDER_TARGET_BLEND_DESC default_render_target_blend_desc = {};
    default_render_target_blend_desc.BlendEnable = FALSE;
    default_render_target_blend_desc.LogicOpEnable = FALSE;
    default_render_target_blend_desc.SrcBlend = D3D12_BLEND_ONE;
    default_render_target_blend_desc.DestBlend = D3D12_BLEND_ZERO;
    default_render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
    default_render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
    default_render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
    default_render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    default_render_target_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
    default_render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
    {
        blend_desc.RenderTarget[i] = default_render_target_blend_desc;
    }
}

void set_rasterizer_state(D3D12_RASTERIZER_DESC& rasterizer_desc)
{
    rasterizer_desc = {};
    rasterizer_desc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizer_desc.CullMode = D3D12_CULL_MODE_BACK;
    rasterizer_desc.FrontCounterClockwise = FALSE;
    rasterizer_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    rasterizer_desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    rasterizer_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    rasterizer_desc.DepthClipEnable = TRUE;
    rasterizer_desc.MultisampleEnable = FALSE;
    rasterizer_desc.AntialiasedLineEnable = FALSE;
    rasterizer_desc.ForcedSampleCount = 0;
    rasterizer_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}

void set_depth_stencil_state(D3D12_DEPTH_STENCIL_DESC& depth_stencil_desc)
{
    depth_stencil_desc = {};

    depth_stencil_desc.DepthEnable = TRUE;
    depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;

    depth_stencil_desc.StencilEnable = FALSE;
    depth_stencil_desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    depth_stencil_desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

    depth_stencil_desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

    depth_stencil_desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
}