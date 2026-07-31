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

// Serializes the root signature, compiles HLSL shaders, and builds the
// Graphics Pipeline State Object (PSO).
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

    // Serialize and create the Root Signature using ComPtr for local blobs.
    Microsoft::WRL::ComPtr<ID3DBlob> signature_blob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;
    HRESULT hr = ::D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature_blob, &error_blob);

    if (FAILED(hr))
    {
        if (error_blob)
            Trace(static_cast<const char*>(error_blob->GetBufferPointer()));

        ::MessageBox(nullptr, L"Root signature serialization failed.", L"Error", MB_OK);
        return false;
    }

    if (signature_blob)
    {
        hr = m_device->CreateRootSignature(0, signature_blob->GetBufferPointer(), signature_blob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
        if (FAILED(hr)) return false;
    }

    // Compile the Vertex Shader (Targeting 5_1 for native DX12).
    Microsoft::WRL::ComPtr<ID3DBlob> vertex_shader = nullptr;
    error_blob = nullptr;
    hr = ::D3DCompileFromFile(L"Shader12.hlsl", nullptr, nullptr, "VSMain", "vs_5_1", 0, 0, &vertex_shader, &error_blob);
    if (FAILED(hr))
    {
        if (error_blob)
        {
            Trace(static_cast<const char*>(error_blob->GetBufferPointer()));
        }
        ::MessageBox(nullptr, L"Vertex shader compilation failed.", L"Shader Error", MB_OK);
        return false;
    }
    error_blob = nullptr; // Reset for next compile pass.

    // Compile the Pixel Shader (Targeting 5_1 for native DX12).
    Microsoft::WRL::ComPtr<ID3DBlob> pixel_shader = nullptr;
    hr = ::D3DCompileFromFile(L"Shader12.hlsl", nullptr, nullptr, "PSMain", "ps_5_1", 0, 0, &pixel_shader, &error_blob);
    if (FAILED(hr))
    {
        if (error_blob)
            Trace(static_cast<const char*>(error_blob->GetBufferPointer()));

        ::MessageBox(nullptr, L"Pixel shader compilation failed.", L"Shader Error", MB_OK);
        return false;
    }

    // Populate and configure the Pipeline State Description (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
    pso_desc.pRootSignature = m_rootSignature.Get();
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

    if (FAILED(hr))
    {
        ::MessageBox(nullptr, L"Pipeline State Object creation failed.", L"PSO Error", MB_OK);
        return false;
    }

    return true;
}

bool CDXView::InitDevice()
{
    // Create the core virtual device representation of the GPU (Targeting native DX12 feature level).
    HRESULT hr = ::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device));
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
    hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList));
    if (FAILED(hr)) return false;

    hr = m_commandList->Close();
    if (FAILED(hr)) return false;

    // Create DXGI Factory using local ComPtr to guarantee zero memory leaks on early returns.
    Microsoft::WRL::ComPtr<IDXGIFactory4> factory = nullptr;
    hr = ::CreateDXGIFactory1(IID_PPV_ARGS(&factory));
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

    Microsoft::WRL::ComPtr<IDXGISwapChain> temp_swap_chain = nullptr;
    hr = factory->CreateSwapChain(m_commandQueue.Get(), &swap_chain_desc, &temp_swap_chain);
    if (FAILED(hr)) return false;

    // Upcast the swap chain to version 3 to leverage modern back buffer indexing.
    hr = temp_swap_chain->QueryInterface(IID_PPV_ARGS(&m_swapChain));
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

        m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtv_handle);
        rtv_handle.ptr += m_rtvIncrementSize;
    }

    // Create the CPU/GPU fence synchronization objects.
    hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    if (FAILED(hr)) return false;

    // Re-enabled wrapper initialization check.
    m_fenceEvent.Attach(::CreateEvent(nullptr, FALSE, FALSE, nullptr));
    if (!m_fenceEvent.IsValid()) return false;

    return true;
}

int CDXView::OnCreate(CREATESTRUCT&)
{
    // Set the window's icon.
    SetIconSmall(IDI_DIRECTX12);
    SetIconLarge(IDI_DIRECTX12);

    // Set the window title.
    SetWindowText(LoadString(IDS_APP_TITLE));

    // Initialize DirectX.
    if (!InitDevice()) { return -1; }
    if (!CreatePipelineState()) { return -1; }

    return 0;
}

// Called when the window is destroyed.
void CDXView::OnDestroy()
{
    // End the application.
    ::PostQuitMessage(0);
}

LRESULT CDXView::OnPaint(UINT, WPARAM, LPARAM)
{
    ValidateRect();
    return 0;
}

LRESULT CDXView::OnSize(UINT, WPARAM, LPARAM lparam)
{
    ResizeBuffers(LOWORD(lparam), HIWORD(lparam));
    return 0;
}

LRESULT CDXView::OnSizing(UINT, WPARAM, LPARAM lparam)
{
    // Extract the target dimensions from the window boundary rect.
    LPRECT rect = reinterpret_cast<LPRECT>(lparam);
    int width = rect->right - rect->left;
    int height = rect->bottom - rect->top;

    // Account for non-client area.
    RECT clientRect = { 0, 0, width, height };
    ::AdjustWindowRectEx(&clientRect, GetStyle(), FALSE, GetExStyle());

    int clientWidth = rect->right - rect->left - (clientRect.right - width);
    int clientHeight = rect->bottom - rect->top - (clientRect.bottom - height);

    // Reallocate DX12 surfaces and instantly push a new frame.
    ResizeBuffers(clientWidth, clientHeight);
    Render();
    return TRUE;
}

// Specifiy the CREATESTRUCT parameters before creating the window.
void CDXView::PreCreate(CREATESTRUCT& cs)
{
    m_width = DpiScaleInt(400);
    m_height = DpiScaleInt(400);
    CRect windowRect(0, 0, m_width, m_height);
    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    cs.cx = windowRect.Width();
    cs.cy = windowRect.Height();
}

void CDXView::Render()
{
    // Prepare command allocators and lists for recording.
    m_commandAllocator->Reset();
    m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get());

    // Identify current active swap chain buffer index.
    UINT backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    // Transition back buffer from PRESENT state to RENDER_TARGET state.
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_renderTargets[backBufferIndex].Get();
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
    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    m_commandList->SetPipelineState(m_pipelineState.Get());

    // Push animation angle and issue the draw call.
    m_commandList->SetGraphicsRoot32BitConstant(0, m_triangleAngle, 0);
    m_commandList->DrawInstanced(3, 1, 0, 0);

    // Transition back buffer back from RENDER_TARGET to PRESENT state.
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_commandList->ResourceBarrier(1, &barrier);

    // Close command recording and dispatch to the GPU command queue.
    m_commandList->Close();

    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(1, ppCommandLists);

    // Present the finished frame to the display.
    m_swapChain->Present(1, 0);

    // CPU/GPU Fence Sync: Wait for the frame execution to finish on the GPU hardware.
    const UINT64 currentFenceValue = ++m_fenceValue;
    m_commandQueue->Signal(m_fence.Get(), currentFenceValue);

    if (m_fence->GetCompletedValue() < currentFenceValue)
    {
        m_fence->SetEventOnCompletion(currentFenceValue, m_fenceEvent.Get());
        ::WaitForSingleObject(m_fenceEvent.Get(), INFINITE);
    }

    // Increment the rotation angle for the next frame's animation.
    m_triangleAngle++;
}

LRESULT CDXView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_PAINT:  return OnPaint(msg, wparam, lparam);
        case WM_SIZE:   return OnSize(msg, wparam, lparam);
        case WM_SIZING: return OnSizing(msg, wparam, lparam);

        default: break;
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << L"Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    return 0;
}

void CDXView::ResizeBuffers(int width, int height)
{
    if (!m_device || width <= 0 || height <= 0) return;

    m_width = width;
    m_height = height;

    // Wait for GPU to finish rendering current work.
    const UINT64 fenceValueToWait = ++m_fenceValue;
    m_commandQueue->Signal(m_fence.Get(), fenceValueToWait);
    if (m_fence->GetCompletedValue() < fenceValueToWait)
    {
        m_fence->SetEventOnCompletion(fenceValueToWait, m_fenceEvent.Get());
        ::WaitForSingleObject(m_fenceEvent.Get(), INFINITE);
    }

    // Drop references to old back buffer resources.
    for (UINT i = 0; i < 2; i++)
    {
        m_renderTargets[i].Reset();
    }

    // Command the DXGI swap chain to reallocate its internal canvas sizes.
    HRESULT hr = m_swapChain->ResizeBuffers(2, m_width, m_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (FAILED(hr)) return;

    // Bind the new back buffer textures back to your Render Target View descriptors.
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < 2; i++)
    {
        hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
        if (FAILED(hr)) return;

        m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
        rtvHandle.ptr += m_rtvIncrementSize;
    }
}


////////////////////
// Global functions.
//

// Configures standard opaque rendering properties for all simultaneous render
// targets.
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

// Configures standard solid geometry rasterization properties with back - face
// culling.
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

// Bypasses depth and stencil testing configurations for flat 2D/procedural rendering.
void set_depth_stencil_state(D3D12_DEPTH_STENCIL_DESC& depth_stencil_desc)
{
    depth_stencil_desc = {};

    depth_stencil_desc.DepthEnable = FALSE;
    depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    depth_stencil_desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

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
