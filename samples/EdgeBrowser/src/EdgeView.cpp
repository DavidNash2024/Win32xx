/////////////////////////////
// EdgeView.cpp
//

#include "stdafx.h"
#include "EdgeView.h"
#include "resource.h"

using namespace Microsoft::WRL;

// OnCreate is called automatically during window creation when a
// WM_CREATE message received.
// Tasks such as setting the icon, creating child windows, or anything
// associated with creating windows are normally performed here.
int CEdgeView::OnCreate(CREATESTRUCT&)
{
    // Set the window's icon
    SetIconSmall(IDI_GLOBE);
    SetIconLarge(IDI_GLOBE);

    // Set the window title
    SetWindowText(LoadString(IDS_CAPTION));

    return 0;
}

// OnDestroy is called when the window is destroyed.
void CEdgeView::OnDestroy()
{
    ::PostQuitMessage(0);
}

// OnInitialUpdate is called after the window is created.
void CEdgeView::OnInitialUpdate()
{
    StartBrowser();
}

// OnSize is called when the window is resized.
LRESULT CEdgeView::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (m_controller != nullptr)
    {
        CRect bounds = GetClientRect();
        m_controller->put_Bounds(bounds);
    }

    return FinalWindowProc(msg, wparam, lparam);
}

// Start the Edge browser.
// Note: This function uses callbacks and completes before m_webView is assigned.
void CEdgeView::StartBrowser()
{
    CString dataDirectory = GetAppDataPath() + L"\\Win32++\\" + LoadString(IDS_APP_TITLE);

    // Use a lambda and callback to create the core web environment.
    CreateCoreWebView2EnvironmentWithOptions(nullptr, dataDirectory, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT, ICoreWebView2Environment* env) -> HRESULT
            {
                // Use a lambda and callback to create the core web controller.
                env->CreateCoreWebView2Controller(*this, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [this](HRESULT, ICoreWebView2Controller* controller) -> HRESULT
                    {
                        if (controller != nullptr)
                        {
                            // Retrieve the web view from the controller.
                            m_controller = controller;
                            m_controller->get_CoreWebView2(&m_webView);

                            // Resize WebView to fit the bounds of the parent window
                            CRect bounds = GetClientRect();
                            m_controller->put_Bounds(bounds);

                            // Navigate to a web site.
                            m_webView->Navigate(L"https://www.google.com/");
                        }
                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());
}

// OnDPIChanged is called when the window DPI has changed.
LRESULT CEdgeView::OnDPIChanged(UINT, WPARAM, LPARAM lparam)
{
	// Resize the window to the recommended size.
    RECT* pWindowRect = reinterpret_cast<RECT*>(lparam);
    assert(pWindowRect);
    SetWindowPos(HWND_TOP, *pWindowRect, SWP_SHOWWINDOW);
    return 0;
}

// WndProc processes the window's messages.
LRESULT CEdgeView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DPICHANGED: return OnDPIChanged(msg, wparam, lparam);
    case WM_SIZE:       return OnSize(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}

