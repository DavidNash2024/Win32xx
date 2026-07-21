/////////////////////////////
// EdgeView.cpp
//

#include "pch.h"
#include "EdgeView.h"
#include "UserMessages.h"
#include <urlmon.h>

#pragma comment(lib, "urlmon.lib")

using namespace Microsoft::WRL;

// CanGoBack returns TRUE if the webview can go back to a previous page.
BOOL CEdgeView::CanGoBack()
{
    BOOL canGoBack = FALSE;
    if (m_webView && SUCCEEDED(m_webView->get_CanGoBack(&canGoBack)) && canGoBack)
        return TRUE;

    return FALSE;
}

// CanGoForward returns TRUE if the webview can go forward to a next page.
BOOL CEdgeView::CanGoForward()
{
    BOOL canGoForward = FALSE;
    if (m_webView && SUCCEEDED(m_webView->get_CanGoForward(&canGoForward)) && canGoForward)
        return TRUE;

    return FALSE;
}

// GoBack navigates the webview to the previous page in the history.
void CEdgeView::GoBack()
{
    BOOL canGoBack = FALSE;
    if (m_webView && SUCCEEDED(m_webView->get_CanGoBack(&canGoBack)) && canGoBack)
        m_webView->GoBack();
}

// GoForward navigates the webview to the next page in the history.
void CEdgeView::GoForward()
{
    BOOL canGoForward = FALSE;
    if (m_webView && SUCCEEDED(m_webView->get_CanGoForward(&canGoForward)) && canGoForward)
        m_webView->GoForward();
}

// Navigate navigates the webview to the specified URL.
void CEdgeView::Navigate(LPCWSTR url)
{
    if (m_webView != nullptr)
    {
        CString urlString(url);
        if (urlString.Find(L"://") == -1)
            urlString = L"https://" + urlString;

        wil::com_ptr<IUri> uri;
        wil::unique_bstr bstrUri;
        if (SUCCEEDED(CreateUri(urlString.c_str(), Uri_CREATE_ALLOW_RELATIVE, 0, &uri)) &&
            (SUCCEEDED(uri->GetAbsoluteUri(&bstrUri))))
        {
            urlString = bstrUri.get();
        }
        else
        {
            urlString = L"https://www.google.com/search?q=" + urlString;
        }

        m_webView->Navigate(urlString);
    }
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

// Reload reloads the current page in the webview.
void CEdgeView::Reload()
{
    if (m_webView)
        m_webView->Reload();
}

// Setup the WebView event handlers.
void CEdgeView::SetupEvents(HWND hWnd)
{
    // Navigation Starting (Cancelable)
    m_webView->add_NavigationStarting(Callback<ICoreWebView2NavigationStartingEventHandler>(
        [hWnd](ICoreWebView2*, ICoreWebView2NavigationStartingEventArgs*) -> HRESULT
        {
            ::SendMessage(hWnd, UWM_NAVIGATIONSTARTED, 0, 0);
            return S_OK;
        }).Get(), nullptr);

    // Source Changed (URL updated)
    m_webView->add_SourceChanged(Callback<ICoreWebView2SourceChangedEventHandler>(
        [hWnd](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs*) -> HRESULT
        {
            wil::unique_cotaskmem_string uri;
            sender->get_Source(&uri);
            ::SendMessage(hWnd, UWM_SOURCECHANGED, 0, (LPARAM)uri.get());
            return S_OK;
        }).Get(), nullptr);

    // History Changed (Back/Forward state)
    m_webView->add_HistoryChanged(Callback<ICoreWebView2HistoryChangedEventHandler>(
        [hWnd](ICoreWebView2*, IUnknown*) -> HRESULT
        {
            ::SendMessage(hWnd, UWM_HISTORYCHANGED, 0, 0);
            return S_OK;
        }).Get(), nullptr);

    // Navigation Completed
    m_webView->add_NavigationCompleted(Callback<ICoreWebView2NavigationCompletedEventHandler>(
        [hWnd](ICoreWebView2*, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
        {
            BOOL success;
            args->get_IsSuccess(&success);
            if (!success) {
                COREWEBVIEW2_WEB_ERROR_STATUS status;
                args->get_WebErrorStatus(&status);
            }
            ::SendMessage(hWnd, UWM_NAVIGATIONCOMPLETED, static_cast<WPARAM>(success), 0);
            return S_OK;
        }).Get(), nullptr);
}

// Start the Edge browser.
// Note: This function uses callbacks and completes before m_webView is assigned.
void CEdgeView::StartBrowser()
{
    CString dataDirectory = GetAppDataPath() + L"\\Win32++\\" + LoadString(IDW_MAIN);

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

                            HWND frame = GetAncestor().GetHwnd();
                            SetupEvents(frame);

                            // Navigate to a web site.
                            m_webView->Navigate(L"https://sourceforge.net/projects/win32-framework/");
                        }
                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());
}

// Stop stops the current navigation in the webview.
void CEdgeView::Stop()
{
    if (m_webView)
        m_webView->Stop();
}

// This function is called by the framework when a message is received.
LRESULT CEdgeView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
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

