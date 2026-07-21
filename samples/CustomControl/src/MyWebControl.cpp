/////////////////////////////
// MyWebControl.cpp
//

#include "pch.h"
#include "MyWebControl.h"
#include "resource.h"

using namespace Microsoft::WRL;

////////////////////////////////////////
// Definitions for the CWebControl class
//

// Constructor
CWebControl::CWebControl()
{
    // The entry for the dialog's control in resource.rc must match this name.
    CString className = "WebControl";

    // Register the window class
    WNDCLASS wc{};
    HINSTANCE instance = GetApp()->GetInstanceHandle();
    if (!::GetClassInfo(instance, className, &wc))
    {
        wc.lpszClassName = className;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.hInstance = instance;
        ::RegisterClass(&wc);
    }

    assert (::GetClassInfo(instance, className, &wc));
}

// OnInitialUpdate is called after the window is created.
void CWebControl::OnInitialUpdate()
{
    StartBrowser();
}

// OnSize is called when the window is resized.
LRESULT CWebControl::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
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
void CWebControl::StartBrowser()
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
                            m_webView->Navigate(L"https://www.google.com.au");
                        }
                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());
}

// OnDPIChanged is called when the window DPI has changed.
LRESULT CWebControl::OnDPIChanged(UINT, WPARAM, LPARAM lparam)
{
    // Resize the window to the recommended size.
    RECT* pWindowRect = reinterpret_cast<RECT*>(lparam);
    assert(pWindowRect);
    SetWindowPos(HWND_TOP, *pWindowRect, SWP_SHOWWINDOW);
    return 0;
}

// WndProc processes the window's messages.
LRESULT CWebControl::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DPICHANGED: return OnDPIChanged(msg, wparam, lparam);
        case WM_SIZE:       return OnSize(msg, wparam, lparam);

        default: return WndProcDefault(msg, wparam, lparam);
        }
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << L"Error: " << e.what();
        TaskDialogBox(nullptr, str1, str2, TD_ERROR_ICON);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        TaskDialogBox(nullptr, str1, L"Error: std::exception", TD_ERROR_ICON);
    }

    return 0;
}

