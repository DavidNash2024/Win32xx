/////////////////////////////
// D2DView.h
//

#pragma once

#include <d2d1.h>       // Provides the core Direct2D API.
#include <wrl/client.h> // Provides the ComPtr smart pointer.

///////////////////////////////////////////////
// CD2DView manages the window that renders the
// Direct2D images.
class CD2DView : public CWnd
{
public:
    CD2DView();
    virtual ~CD2DView() override;

protected:
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnDestroy() override;
    virtual LRESULT OnDpiChanged(UINT, WPARAM, LPARAM);
    virtual LRESULT OnPaint(UINT, WPARAM, LPARAM) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual void    PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CD2DView(const CD2DView&) = delete;
    CD2DView& operator=(const CD2DView&) = delete;

    LRESULT OnDisplayChange(UINT, WPARAM, LPARAM);
    LRESULT OnSize(UINT, WPARAM wparam, LPARAM lparm);

    // Initialization routines
    HRESULT CreateDeviceIndependentResources();
    HRESULT CreateDeviceResources();
    void    DiscardDeviceResources();
    HRESULT OnRender();
    void    OnResize(UINT width, UINT height);

    // Device-Independent Resource (Lives for the lifetime of the window)
    Microsoft::WRL::ComPtr<ID2D1Factory>          m_pDirect2dFactory;

    // Device-Dependent Resources (Can be discarded and recreated dynamically)
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_pRenderTarget;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  m_pLightSlateGrayBrush;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  m_pCornflowerBlueBrush;
};