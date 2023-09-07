/////////////////////////////
// EdgeView.cpp
//

#include "stdafx.h"
#include "EdgeView.h"

using namespace Microsoft::WRL;

// OnCreate is called automatically during window creation when a
// WM_CREATE message received.
// Tasks such as setting the icon, creating child windows, or anything
// associated with creating windows are normally performed here.
int CEdgeView::OnCreate(CREATESTRUCT&)
{
    // Set the window's icon
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Set the window title
    SetWindowText(LoadString(IDW_MAIN));

    return 0;
}

// OnDestroy is called when the window is destroyed.
void CEdgeView::OnDestroy()
{
    ::PostQuitMessage(0);
}

// OnOnitialUpdate is called after the window is created.
void CEdgeView::OnInitialUpdate()
{
    StartBrowser();
}

// Start the Edge browser.
// This function uses callbacks and completes before m_webView is assigned.
void CEdgeView::StartBrowser()
{
    // Use a lambda function to create the core web environment.
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT, ICoreWebView2Environment* env) -> HRESULT
            {
                // Use a lambda function to create the core web controller.
                env->CreateCoreWebView2Controller(*this, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [this](HRESULT, ICoreWebView2Controller* controller) -> HRESULT
                    {
                        // Retrieve the web vire from the controller.
                        if (controller != nullptr)
                        {
                            m_controller = controller;
                            m_controller->get_CoreWebView2(&m_webView);
                        }

                        // Resize WebView to fit the bounds of the parent window
                        CRect bounds = GetClientRect();
                        m_controller->put_Bounds(bounds);

                        // Navigate to a web site.
                        m_webView->Navigate(L"https://www.google.com/");

                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());
}

LRESULT CEdgeView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_SIZE:
        if (m_controller != nullptr)
        {
            CRect bounds = GetClientRect();
            m_controller->put_Bounds(bounds);
        }
        break;

    default:
        return WndProcDefault(msg, wparam, lparam);
        break;
    }

    return 0;
}

