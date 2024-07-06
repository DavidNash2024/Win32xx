// Copyright (C) Microsoft Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Ackowledgement:
// This sample uses the 'JSON for Modern C++' library, version 3.11.2
// by Niels Lohmann <https://nlohmann.me>
// https://github.com/nlohmann/json

#include "stdafx.h"
#include "BrowserWindow.h"
#include "Tab.h"

#pragma comment (lib, "Urlmon.lib")

using namespace Microsoft::WRL;
using namespace nlohmann;

void CBrowserWindow::CheckFailure(HRESULT hr, LPCWSTR errorMessage)
{
    if (FAILED(hr))
    {
        std::wstring message;
        if (!errorMessage || !errorMessage[0])
        {
            message = std::wstring(L"Something went wrong.");
        }
        else
        {
            message = std::wstring(errorMessage);
        }

        ::MessageBoxW(nullptr, message.c_str(), nullptr, MB_OK);
    }
}

HRESULT CBrowserWindow::ClearContentCache()
{
    return m_tabs.at(m_activeTabId)->m_contentWebView->CallDevToolsProtocolMethod(L"Network.clearBrowserCache", L"{}", nullptr);
}

HRESULT CBrowserWindow::ClearControlsCache()
{
    return m_controlsWebView->CallDevToolsProtocolMethod(L"Network.clearBrowserCache", L"{}", nullptr);
}

HRESULT CBrowserWindow::ClearContentCookies()
{
    return m_tabs.at(m_activeTabId)->m_contentWebView->CallDevToolsProtocolMethod(L"Network.clearBrowserCookies", L"{}", nullptr);
}

HRESULT CBrowserWindow::ClearControlsCookies()
{
    return m_controlsWebView->CallDevToolsProtocolMethod(L"Network.clearBrowserCookies", L"{}", nullptr);
}

HRESULT CBrowserWindow::CreateBrowserControlsWebView()
{
    return m_uiEnv->CreateCoreWebView2Controller(*this, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
        [this](HRESULT result, ICoreWebView2Controller* host) -> HRESULT
        {
            if (!SUCCEEDED(result))
            {
                OutputDebugString(L"Controls WebView creation failed\n");
                return result;
            }

            // Store the control's controller and the control's webview.
            m_controlsController = host;
            CheckFailure(m_controlsController->get_CoreWebView2(&m_controlsWebView), L"");

            wil::com_ptr<ICoreWebView2Settings> settings;
            RETURN_IF_FAILED(m_controlsWebView->get_Settings(&settings));
            RETURN_IF_FAILED(settings->put_AreDevToolsEnabled(FALSE));

            RETURN_IF_FAILED(m_controlsController->add_ZoomFactorChanged(Callback<ICoreWebView2ZoomFactorChangedEventHandler>(
                [](ICoreWebView2Controller* host, IUnknown* args) -> HRESULT
                {
                    host->put_ZoomFactor(1.0);
                    return S_OK;
                }
            ).Get(), &m_controlsZoomToken));

            RETURN_IF_FAILED(m_controlsWebView->add_WebMessageReceived(m_uiMessageBroker.Get(), &m_controlsUIMessageBrokerToken));
            RETURN_IF_FAILED(ResizeUIWebViews());

            std::wstring controlsPath = GetFullPathFor(L"wvbrowser_ui\\controls_ui\\default.html");
            RETURN_IF_FAILED(m_controlsWebView->Navigate(controlsPath.c_str()));

            return S_OK;
        }).Get());
}

HRESULT CBrowserWindow::CreateBrowserOptionsWebView()
{
    return m_uiEnv->CreateCoreWebView2Controller(*this, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
        [this](HRESULT result, ICoreWebView2Controller* host) -> HRESULT
        {
            if (!SUCCEEDED(result))
            {
                OutputDebugString(L"Options WebView creation failed\n");
                return result;
            }

            // Store the option's controller and the option's webview.
            m_optionsController = host;
            CheckFailure(m_optionsController->get_CoreWebView2(&m_optionsWebView), L"");

            wil::com_ptr<ICoreWebView2Settings> settings;
            RETURN_IF_FAILED(m_optionsWebView->get_Settings(&settings));
            RETURN_IF_FAILED(settings->put_AreDevToolsEnabled(FALSE));

            RETURN_IF_FAILED(m_optionsController->add_ZoomFactorChanged(Callback<ICoreWebView2ZoomFactorChangedEventHandler>(
                [](ICoreWebView2Controller* host, IUnknown* args) -> HRESULT
                {
                    host->put_ZoomFactor(1.0);
                    return S_OK;
                }
            ).Get(), &m_optionsZoomToken));

            // Hide by default
            RETURN_IF_FAILED(m_optionsController->put_IsVisible(FALSE));
            RETURN_IF_FAILED(m_optionsWebView->add_WebMessageReceived(m_uiMessageBroker.Get(), &m_optionsUIMessageBrokerToken));

            // Hide menu when focus is lost
            RETURN_IF_FAILED(m_optionsController->add_LostFocus(Callback<ICoreWebView2FocusChangedEventHandler>(
                [this](ICoreWebView2Controller* sender, IUnknown* args) -> HRESULT
                {
                    json jsonObj;
                    jsonObj["message"] = MG_OPTIONS_LOST_FOCUS;

                    PostJsonToWebView(jsonObj, m_controlsWebView.Get());

                    return S_OK;
                }).Get(), &m_lostOptionsFocus));

            RETURN_IF_FAILED(ResizeUIWebViews());

            std::wstring optionsPath = GetFullPathFor(L"wvbrowser_ui\\controls_ui\\options.html");
            RETURN_IF_FAILED(m_optionsWebView->Navigate(optionsPath.c_str()));

            return S_OK;
        }).Get());
}

std::wstring CBrowserWindow::GetAppDataDirectory()
{
    TCHAR path[MAX_PATH];
    std::wstring dataDirectory;
    HRESULT hr = SHGetFolderPath(nullptr, CSIDL_APPDATA, NULL, 0, path);
    if (SUCCEEDED(hr))
    {
        dataDirectory = std::wstring(path);
        dataDirectory.append(L"\\Win32++\\");
    }
    else
    {
        dataDirectory = std::wstring(L".\\");
    }

    CString title = LoadString(IDS_APP_TITLE);
    dataDirectory.append(title);
    return dataDirectory;
}

int CBrowserWindow::GetDPIAwareBound(int bound)
{
    return (bound * GetWindowDpi(*this) / DEFAULT_DPI);
}

std::wstring CBrowserWindow::GetFilePathAsURI(std::wstring fullPath)
{
    std::wstring fileURI;
    ComPtr<IUri> uri;
    DWORD uriFlags = Uri_CREATE_ALLOW_IMPLICIT_FILE_SCHEME;
    HRESULT hr = CreateUri(fullPath.c_str(), uriFlags, 0, &uri);

    if (SUCCEEDED(hr))
    {
        wil::unique_bstr absoluteUri;
        uri->GetAbsoluteUri(&absoluteUri);
        fileURI = std::wstring(absoluteUri.get());
    }

    return fileURI;
}

std::wstring CBrowserWindow::GetFullPathFor(LPCWSTR relativePath)
{
    WCHAR path[MAX_PATH];
    HINSTANCE instance = GetApp()->GetInstanceHandle();
    GetModuleFileNameW(instance, path, MAX_PATH);
    std::wstring pathName(path);

    std::size_t index = pathName.find_last_of(L"\\") + 1;
    pathName.replace(index, pathName.length(), relativePath);

    return pathName;
}

void CBrowserWindow::HandleTabCreated(size_t tabId, bool shouldBeActive)
{
    if (shouldBeActive)
    {
        CheckFailure(SwitchToTab(tabId), L"");
    }
}

HRESULT CBrowserWindow::HandleTabHistoryUpdate(size_t tabId, ICoreWebView2* webview)
{
    wil::unique_cotaskmem_string source;
    RETURN_IF_FAILED(webview->get_Source(&source));

    json jsonObj;
    jsonObj["message"] = MG_UPDATE_URI;
    jsonObj["args"]["tabId"] = tabId;
    jsonObj["args"]["uri"] = std::string(WtoA(source.get(), CP_UTF8).c_str());

    BOOL canGoForward = FALSE;
    RETURN_IF_FAILED(webview->get_CanGoForward(&canGoForward));
    jsonObj["args"]["canGoForward"] = canGoForward;

    BOOL canGoBack = FALSE;
    RETURN_IF_FAILED(webview->get_CanGoBack(&canGoBack));
    jsonObj["args"]["canGoBack"] = canGoBack;

    RETURN_IF_FAILED(PostJsonToWebView(jsonObj, m_controlsWebView.Get()));

    return S_OK;
}

HRESULT CBrowserWindow::HandleTabMessageReceived(size_t tabId, ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* eventArgs)
{
    wil::unique_cotaskmem_string jsonString;
    RETURN_IF_FAILED(eventArgs->get_WebMessageAsJson(&jsonString));

    json jsonObj = json::parse(WtoA(jsonString.get(), CP_UTF8).c_str());

    wil::unique_cotaskmem_string uri;
    RETURN_IF_FAILED(webview->get_Source(&uri));

    int message = jsonObj["message"].get<int>();

    wil::unique_cotaskmem_string source;
    RETURN_IF_FAILED(webview->get_Source(&source));

    switch (message)
    {
    case MG_GET_FAVORITES:
    case MG_REMOVE_FAVORITE:
    {
        std::wstring fileURI = GetFilePathAsURI(GetFullPathFor(L"wvbrowser_ui\\content_ui\\favorites.html"));
        // Only the favorites UI can request favorites
        if (fileURI.compare(source.get()) == 0)
        {
            jsonObj["args"]["tabId"] = tabId;
            CheckFailure(PostJsonToWebView(jsonObj, m_controlsWebView.Get()), L"Couldn't perform favorites operation.");
        }
    }
    break;
    case MG_GET_SETTINGS:
    {
        std::wstring fileURI = GetFilePathAsURI(GetFullPathFor(L"wvbrowser_ui\\content_ui\\settings.html"));
        // Only the settings UI can request settings
        if (fileURI.compare(source.get()) == 0)
        {
            jsonObj["args"]["tabId"] = tabId;
            CheckFailure(PostJsonToWebView(jsonObj, m_controlsWebView.Get()), L"Couldn't retrieve settings.");
        }
    }
    break;
    case MG_CLEAR_CACHE:
    {
        std::wstring fileURI = GetFilePathAsURI(GetFullPathFor(L"wvbrowser_ui\\content_ui\\settings.html"));
        // Only the settings UI can request cache clearing
        if (fileURI.compare(uri.get()) == 0)
        {
            jsonObj["args"]["content"] = false;
            jsonObj["args"]["controls"] = false;

            if (SUCCEEDED(ClearContentCache()))
            {
                jsonObj["args"]["content"] = true;
            }

            if (SUCCEEDED(ClearControlsCache()))
            {
                jsonObj["args"]["controls"] = true;
            }

            CheckFailure(PostJsonToWebView(jsonObj, m_tabs.at(tabId)->m_contentWebView.Get()), L"");
        }
    }
    break;
    case MG_CLEAR_COOKIES:
    {
        std::wstring fileURI = GetFilePathAsURI(GetFullPathFor(L"wvbrowser_ui\\content_ui\\settings.html"));
        // Only the settings UI can request cookies clearing
        if (fileURI.compare(uri.get()) == 0)
        {
            jsonObj["args"]["content"] = false;
            jsonObj["args"]["controls"] = false;

            if (SUCCEEDED(ClearContentCookies()))
            {
                jsonObj["args"]["content"] = true;
            }


            if (SUCCEEDED(ClearControlsCookies()))
            {
                jsonObj["args"]["controls"] = true;
            }

            CheckFailure(PostJsonToWebView(jsonObj, m_tabs.at(tabId)->m_contentWebView.Get()), L"");
        }
    }
    break;
    case MG_GET_HISTORY:
    case MG_REMOVE_HISTORY_ITEM:
    case MG_CLEAR_HISTORY:
    {
        std::wstring fileURI = GetFilePathAsURI(GetFullPathFor(L"wvbrowser_ui\\content_ui\\history.html"));
        // Only the history UI can request history
        if (fileURI.compare(uri.get()) == 0)
        {
            jsonObj["args"]["tabId"] = tabId;
            CheckFailure(PostJsonToWebView(jsonObj, m_controlsWebView.Get()), L"Couldn't perform history operation");
        }
    }
    break;
    default:
    {
        OutputDebugString(L"Unexpected message\n");
    }
    break;
    }

    return S_OK;
}

HRESULT CBrowserWindow::HandleTabNavCompleted(size_t tabId, ICoreWebView2* webview, ICoreWebView2NavigationCompletedEventArgs* args)
{
    std::wstring getTitleScript(
        // Look for a title tag
        L"(() => {"
        L"    const titleTag = document.getElementsByTagName('title')[0];"
        L"    if (titleTag) {"
        L"        return titleTag.innerHTML;"
        L"    }"
        // No title tag, look for the file name
        L"    pathname = window.location.pathname;"
        L"    var filename = pathname.split('/').pop();"
        L"    if (filename) {"
        L"        return filename;"
        L"    }"
        // No file name, look for the hostname
        L"    const hostname =  window.location.hostname;"
        L"    if (hostname) {"
        L"        return hostname;"
        L"    }"
        // Fallback: let the UI use a generic title
        L"    return '';"
        L"})();"
    );

    std::wstring getFaviconURI(
        L"(() => {"
        // Let the UI use a fallback favicon
        L"    let faviconURI = '';"
        L"    let links = document.getElementsByTagName('link');"
        // Test each link for a favicon
        L"    Array.from(links).map(element => {"
        L"        let rel = element.rel;"
        // Favicon is declared, try to get the href
        L"        if (rel && (rel == 'shortcut icon' || rel == 'icon')) {"
        L"            if (!element.href) {"
        L"                return;"
        L"            }"
        // href to icon found, check it's full URI
        L"            try {"
        L"                let urlParser = new URL(element.href);"
        L"                faviconURI = urlParser.href;"
        L"            } catch(e) {"
        // Try prepending origin
        L"                let origin = window.location.origin;"
        L"                let faviconLocation = `${origin}/${element.href}`;"
        L"                try {"
        L"                    urlParser = new URL(faviconLocation);"
        L"                    faviconURI = urlParser.href;"
        L"                } catch (e2) {"
        L"                    return;"
        L"                }"
        L"            }"
        L"        }"
        L"    });"
        L"    return faviconURI;"
        L"})();"
    );

    CheckFailure(webview->ExecuteScript(getTitleScript.c_str(), Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
        [this, tabId](HRESULT error, PCWSTR result) -> HRESULT
        {
            RETURN_IF_FAILED(error);

            json jsonObj;
            jsonObj["message"] = MG_UPDATE_TAB;
            jsonObj["args"]["title"] = std::string(WtoA(result, CP_UTF8).c_str());
            jsonObj["args"]["tabId"] = tabId;

            CheckFailure(PostJsonToWebView(jsonObj, m_controlsWebView.Get()), L"Can't update title.");
            return S_OK;
        }).Get()), L"Can't update title.");

    CheckFailure(webview->ExecuteScript(getFaviconURI.c_str(), Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
        [this, tabId](HRESULT error, PCWSTR result) -> HRESULT
        {
            RETURN_IF_FAILED(error);

            json jsonObj;
            jsonObj["message"] = MG_UPDATE_FAVICON;
            jsonObj["args"]["uri"] = std::string(WtoA(result, CP_UTF8).c_str());
            jsonObj["args"]["tabId"] = tabId;

            CheckFailure(PostJsonToWebView(jsonObj, m_controlsWebView.Get()), L"Can't update favicon.");
            return S_OK;
        }).Get()), L"Can't update favicon");

    json jsonObj;
    jsonObj["message"] = MG_NAV_COMPLETED;
    jsonObj["args"]["tabId"] = tabId;

    BOOL navigationSucceeded = FALSE;
    if (SUCCEEDED(args->get_IsSuccess(&navigationSucceeded)))
    {
        jsonObj["args"]["isError"] = !navigationSucceeded;
    }

    return PostJsonToWebView(jsonObj, m_controlsWebView.Get());
}

HRESULT CBrowserWindow::HandleTabNavStarting(size_t tabId, ICoreWebView2* webview)
{
    json jsonObj;
    jsonObj["message"] = MG_NAV_STARTING;
    jsonObj["args"]["tabId"] = tabId;

    return PostJsonToWebView(jsonObj, m_controlsWebView.Get());
}

HRESULT CBrowserWindow::HandleTabSecurityUpdate(size_t tabId, ICoreWebView2* webview, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args)
{
    wil::unique_cotaskmem_string jsonArgs;
    RETURN_IF_FAILED(args->get_ParameterObjectAsJson(&jsonArgs));
    json securityEvent = json::parse(std::string(WtoA(jsonArgs.get(), CP_UTF8)));

    json jsonObj;
    jsonObj["message"] = MG_SECURITY_UPDATE;
    jsonObj["args"]["tabId"] = tabId;
    jsonObj["args"]["state"] = securityEvent.at("securityState");

    return PostJsonToWebView(jsonObj, m_controlsWebView.Get());
}

HRESULT CBrowserWindow::HandleTabURIUpdate(size_t tabId, ICoreWebView2* webview)
{
    wil::unique_cotaskmem_string source;
    RETURN_IF_FAILED(webview->get_Source(&source));

    json jsonObj;
    jsonObj["message"] = MG_UPDATE_URI;
    jsonObj["args"]["tabId"] = tabId;
    jsonObj["args"]["uri"] = std::string(WtoA(source.get(), CP_UTF8).c_str());

    int message = jsonObj["message"].get<int>();

    std::wstring uri(source.get());
    std::wstring favoritesURI = GetFilePathAsURI(GetFullPathFor(L"wvbrowser_ui\\content_ui\\favorites.html"));
    std::wstring settingsURI = GetFilePathAsURI(GetFullPathFor(L"wvbrowser_ui\\content_ui\\settings.html"));
    std::wstring historyURI = GetFilePathAsURI(GetFullPathFor(L"wvbrowser_ui\\content_ui\\history.html"));

    if (uri.compare(favoritesURI) == 0)
    {
        jsonObj["args"]["uriToShow"] = "browser://favorites";
    }
    else if (uri.compare(settingsURI) == 0)
    {
        jsonObj["args"]["uriToShow"] = "browser://settings";
    }
    else if (uri.compare(historyURI) == 0)
    {
        jsonObj["args"]["uriToShow"] = "browser://history";
    }

    RETURN_IF_FAILED(PostJsonToWebView(jsonObj, m_controlsWebView.Get()));

    return S_OK;
}

HRESULT CBrowserWindow::InitUIWebViews()
{
    // Get data directory for browser UI data
    std::wstring browserDataDirectory = GetAppDataDirectory();
    browserDataDirectory.append(L"\\Browser Data");

    // Create WebView environment for browser UI. A separate data directory is
    // used to isolate the browser UI from web content requested by the user.
    return CreateCoreWebView2EnvironmentWithOptions(nullptr, browserDataDirectory.c_str(),
        nullptr, Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
    {
        // Environment is ready, create the WebView
        m_uiEnv = env;

        RETURN_IF_FAILED(CreateBrowserControlsWebView());
        RETURN_IF_FAILED(CreateBrowserOptionsWebView());

        return S_OK;
    }).Get());
}

BOOL CBrowserWindow::LaunchBrowser()
{
    UpdateMinWindowSize();

    SetUIMessageBroker();

    // Get directory for user data. This will be kept separated from the
    // directory for the browser UI data.
    std::wstring userDataDirectory = GetAppDataDirectory();
    userDataDirectory.append(L"\\User Data");

    // Create WebView environment for web content requested by the user. All
    // tabs will be created from this environment and kept isolated from the
    // browser UI. This environment is created first so the UI can request new
    // tabs when it's ready.
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(nullptr, userDataDirectory.c_str(),
        nullptr, Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                RETURN_IF_FAILED(result);

                m_contentEnv = env;
                HRESULT hr = InitUIWebViews();

                if (!SUCCEEDED(hr))
                {
                    OutputDebugString(L"UI WebViews environment creation failed\n");
                }

                return hr;
            }).Get());

    if (!SUCCEEDED(hr))
    {
        OutputDebugString(L"Content WebViews environment creation failed\n");
        return FALSE;
    }

    return TRUE;
}

void CBrowserWindow::OnClose()
{
    json jsonObj;
    jsonObj["message"] = MG_CLOSE_WINDOW;

    CheckFailure(PostJsonToWebView(jsonObj, m_controlsWebView.Get()), L"Try again.");

    CWnd::OnClose();
}


int CBrowserWindow::OnCreate(CREATESTRUCT& cs)
{
    // Set the window's icon
    SetIconSmall(IDI_GLOBE);
    SetIconLarge(IDI_GLOBE);

    // Set the window title
    SetWindowText(LoadString(IDS_CAPTION));

    // Create the tabs and start the browser.
    if (!LaunchBrowser())
        ::MessageBox(nullptr, L"Failed to launch browser", L"Error", MB_OK);

    return 0;
}

void CBrowserWindow::OnDestroy()
{
    ::PostQuitMessage(0);
}

LRESULT CBrowserWindow::OnDPIChanged(UINT, WPARAM, LPARAM lparam)
{
    UpdateMinWindowSize();

    RECT* pWindowRect = reinterpret_cast<RECT*>(lparam);
    assert(pWindowRect);
    SetWindowPos(HWND_TOP, *pWindowRect, SWP_SHOWWINDOW);

    return 0;
}

LRESULT CBrowserWindow::OnGetMinMaxInfo(UINT, WPARAM, LPARAM lparam)
{
    MINMAXINFO* minmax = reinterpret_cast<MINMAXINFO*>(lparam);
    minmax->ptMinTrackSize.x = m_minWindowWidth;
    minmax->ptMinTrackSize.y = m_minWindowHeight;
    return 0;
}

LRESULT CBrowserWindow::OnSize(UINT, WPARAM, LPARAM)
{
    ResizeUIWebViews();
    if (m_tabs.find(m_activeTabId) != m_tabs.end())
    {
        m_tabs.at(m_activeTabId)->ResizeWebView();
    }

    return 0;
}

HRESULT CBrowserWindow::PostJsonToWebView(json jsonObj, ICoreWebView2* webview)
{
    std::stringstream stream;
    stream << jsonObj;

    return webview->PostWebMessageAsJson(AtoW(stream.str().c_str(), CP_UTF8));
}

HRESULT CBrowserWindow::ResizeUIWebViews()
{
    if (m_controlsWebView != nullptr)
    {
        CRect bounds = GetClientRect();
        bounds.bottom = bounds.top + GetDPIAwareBound(c_uiBarHeight);
        bounds.bottom += 1;

        RETURN_IF_FAILED(m_controlsController->put_Bounds(bounds));
    }

    if (m_optionsWebView != nullptr)
    {
        CRect bounds = GetClientRect();
        bounds.top = GetDPIAwareBound(c_uiBarHeight);
        bounds.bottom = bounds.top + GetDPIAwareBound(c_optionsDropdownHeight);
        bounds.left = bounds.right - GetDPIAwareBound(c_optionsDropdownWidth);

        RETURN_IF_FAILED(m_optionsController->put_Bounds(bounds));
    }

    // Workaround for black controls WebView issue in Windows 7
    HWND wvWindow = ::GetWindow(*this, GW_CHILD);
    while (wvWindow != nullptr)
    {
        ::UpdateWindow(wvWindow);
        wvWindow = ::GetWindow(wvWindow, GW_HWNDNEXT);
    }

    return S_OK;
}

// Set the message broker for the UI webview. This will capture messages from ui web content.
// Lambda is used to capture the instance while satisfying Microsoft::WRL::Callback<T>()
void CBrowserWindow::SetUIMessageBroker()
{
    m_uiMessageBroker = Callback<ICoreWebView2WebMessageReceivedEventHandler>(
        [this](ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* eventArgs) -> HRESULT
    {
        wil::unique_cotaskmem_string jsonString;
        CheckFailure(eventArgs->get_WebMessageAsJson(&jsonString), L"");  // Get the message from the UI WebView as JSON formatted string
        json jsonObj = json::parse(WtoA(jsonString.get(), CP_UTF8).c_str());

        if (!jsonObj.contains("message"))
        {
            OutputDebugString(L"No message code provided\n");
            return S_OK;
        }

        if (!jsonObj.contains("args"))
        {
            OutputDebugString(L"The message has no args field\n");
            return S_OK;
        }

        int message = jsonObj.at("message").get<int>();
        json args = jsonObj.at("args");

        switch (message)
        {
        case MG_CREATE_TAB:
        {
            size_t id = args.at("tabId").get<size_t>();
            bool shouldBeActive = args.at("active").get<bool>();
            std::unique_ptr<Tab> newTab = Tab::CreateNewTab(*this, m_contentEnv.Get(), id, shouldBeActive);

            std::map<size_t, std::unique_ptr<Tab>>::iterator it = m_tabs.find(id);
            if (it == m_tabs.end())
            {
                m_tabs.insert(std::pair<size_t,std::unique_ptr<Tab>>(id, std::move(newTab)));
            }
            else
            {
                m_tabs.at(id)->m_contentController->Close();
                it->second = std::move(newTab);
            }
        }
        break;
        case MG_NAVIGATE:
        {
            std::string uriA(args.at("uri").get<std::string>());
            std::wstring uri(AtoW(uriA.c_str(), CP_UTF8));
            std::wstring browserScheme(L"browser://");

            if (uri.substr(0, browserScheme.size()).compare(browserScheme) == 0)
            {
                // No encoded search URI
                std::wstring path = uri.substr(browserScheme.size());
                if (path.compare(L"favorites") == 0 ||
                    path.compare(L"settings") == 0 ||
                    path.compare(L"history") == 0)
                {
                    std::wstring filePath(L"wvbrowser_ui\\content_ui\\");
                    filePath.append(path);
                    filePath.append(L".html");
                    std::wstring fullPath = GetFullPathFor(filePath.c_str());
                    CheckFailure(m_tabs.at(m_activeTabId)->m_contentWebView->Navigate(fullPath.c_str()), L"Can't navigate to browser page.");
                }
                else
                {
                    OutputDebugString(L"Requested unknown browser page\n");
                }
            }
            else if (!SUCCEEDED(m_tabs.at(m_activeTabId)->m_contentWebView->Navigate(uri.c_str())))
            {
                std::string uri = args.at("encodedSearchURI").get<std::string>().c_str();
                CheckFailure(m_tabs.at(m_activeTabId)->m_contentWebView->Navigate(AtoW(uri.c_str(), CP_UTF8)), L"Can't navigate to requested page.");
            }
        }
        break;
        case MG_GO_FORWARD:
        {
            CheckFailure(m_tabs.at(m_activeTabId)->m_contentWebView->GoForward(), L"");
        }
        break;
        case MG_GO_BACK:
        {
            CheckFailure(m_tabs.at(m_activeTabId)->m_contentWebView->GoBack(), L"");
        }
        break;
        case MG_RELOAD:
        {
            CheckFailure(m_tabs.at(m_activeTabId)->m_contentWebView->Reload(), L"");
        }
        break;
        case MG_CANCEL:
        {
            CheckFailure(m_tabs.at(m_activeTabId)->m_contentWebView->CallDevToolsProtocolMethod(L"Page.stopLoading", L"{}", nullptr), L"");
        }
        break;
        case MG_SWITCH_TAB:
        {
            size_t tabId = args.at("tabId").get<size_t>();

            SwitchToTab(tabId);
        }
        break;
        case MG_CLOSE_TAB:
        {
            size_t id = args.at("tabId").get<size_t>();
            m_tabs.at(id)->m_contentController->Close();
            m_tabs.erase(id);
        }
        break;
        case MG_CLOSE_WINDOW:
        {
            DestroyWindow(*this);
        }
        break;
        case MG_SHOW_OPTIONS:
        {
            CheckFailure(m_optionsController->put_IsVisible(TRUE), L"");
            m_optionsController->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }
        break;
        case MG_HIDE_OPTIONS:
        {
            CheckFailure(m_optionsController->put_IsVisible(FALSE), L"Something went wrong when trying to close the options dropdown.");
        }
        break;
        case MG_OPTION_SELECTED:
        {
            m_tabs.at(m_activeTabId)->m_contentController->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        }
        break;
        case MG_GET_FAVORITES:
        case MG_GET_SETTINGS:
        case MG_GET_HISTORY:
        {
            // Forward back to requesting tab
            size_t tabId = args.at("tabId").get<size_t>();
            jsonObj["args"].erase("tabId");

            CheckFailure(PostJsonToWebView(jsonObj, m_tabs.at(tabId)->m_contentWebView.Get()), L"Requesting history failed.");
        }
        break;
        default:
        {
            OutputDebugString(L"Unexpected message\n");
        }
        break;
        }

        return S_OK;
    });
}

HRESULT CBrowserWindow::SwitchToTab(size_t tabId)
{
    size_t previousActiveTab = m_activeTabId;

    RETURN_IF_FAILED(m_tabs.at(tabId)->ResizeWebView());
    RETURN_IF_FAILED(m_tabs.at(tabId)->m_contentController->put_IsVisible(TRUE));
    m_activeTabId = tabId;

    if (previousActiveTab != INVALID_TAB_ID && previousActiveTab != m_activeTabId) {
        auto previousTabIterator = m_tabs.find(previousActiveTab);
        if (previousTabIterator != m_tabs.end() && previousTabIterator->second &&
            previousTabIterator->second->m_contentController)
        {
            auto hr = m_tabs.at(previousActiveTab)->m_contentController->put_IsVisible(FALSE);
            if (hr == HRESULT_FROM_WIN32(ERROR_INVALID_STATE)) {
                json jsonObj;
                jsonObj["message"] = MG_CLOSE_TAB;
                jsonObj["args"]["tabId"] = previousActiveTab;

                PostJsonToWebView(jsonObj, m_controlsWebView.Get());
            }
            RETURN_IF_FAILED(hr);
        }
    }

    return S_OK;
}

void CBrowserWindow::UpdateMinWindowSize()
{
    CRect clientRect = GetClientRect();
    CRect windowRect = GetWindowRect();

    int bordersWidth = (windowRect.right - windowRect.left) - clientRect.right;
    int bordersHeight = (windowRect.bottom - windowRect.top) - clientRect.bottom;

    m_minWindowWidth = GetDPIAwareBound(MIN_WINDOW_WIDTH) + bordersWidth;
    m_minWindowHeight = GetDPIAwareBound(MIN_WINDOW_HEIGHT) + bordersHeight;
}

// Process the window messages.
LRESULT CBrowserWindow::WndProc(UINT message, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (message)
        {
        case WM_GETMINMAXINFO:  return OnGetMinMaxInfo(message, wparam, lparam);
        case WM_DPICHANGED:     return OnDPIChanged(message, wparam, lparam);
        case WM_SIZE:           return OnSize(message, wparam, lparam);
        default:                return WndProcDefault(message, wparam, lparam);
        }
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;

}
