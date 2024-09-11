// Copyright (C) Microsoft Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "framework.h"
#include "Tab.h"


class CBrowserWindow : public CWnd
{
public:
    static const int c_uiBarHeight = 70;
    static const int c_optionsDropdownHeight = 108;
    static const int c_optionsDropdownWidth = 200;

    static std::wstring GetAppDataDirectory();
    BOOL LaunchBrowser();
    std::wstring GetFullPathFor(LPCWSTR relativePath);
    HRESULT HandleTabURIUpdate(size_t tabId, ICoreWebView2* webview);
    HRESULT HandleTabHistoryUpdate(size_t tabId, ICoreWebView2* webview);
    HRESULT HandleTabNavStarting(size_t tabId, ICoreWebView2* webview);
    HRESULT HandleTabNavCompleted(size_t tabId, ICoreWebView2* webview, ICoreWebView2NavigationCompletedEventArgs* args);
    HRESULT HandleTabSecurityUpdate(size_t tabId, ICoreWebView2* webview, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args);
    void HandleTabCreated(size_t tabId, bool shouldBeActive);
    HRESULT HandleTabMessageReceived(size_t tabId, ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* eventArgs);
    int GetDPIAwareBound(int bound);
    static void CheckFailure(HRESULT hr, LPCWSTR errorMessage);

protected:
    virtual void OnClose() override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnDestroy() override;
    virtual LRESULT OnDPIChanged(UINT, WPARAM, LPARAM lparam);
    virtual LRESULT OnGetMinMaxInfo(UINT, WPARAM, LPARAM lparam);
    virtual LRESULT OnSize(UINT, WPARAM, LPARAM);
    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam) override;

private:
    int m_minWindowWidth = 0;
    int m_minWindowHeight = 0;

    Microsoft::WRL::ComPtr<ICoreWebView2Environment> m_uiEnv;
    Microsoft::WRL::ComPtr<ICoreWebView2Environment> m_contentEnv;
    Microsoft::WRL::ComPtr<ICoreWebView2Controller> m_controlsController;
    Microsoft::WRL::ComPtr<ICoreWebView2Controller> m_optionsController;
    Microsoft::WRL::ComPtr<ICoreWebView2> m_controlsWebView;
    Microsoft::WRL::ComPtr<ICoreWebView2> m_optionsWebView;
    std::map<size_t,std::unique_ptr<Tab>> m_tabs;
    size_t m_activeTabId = 0;

    EventRegistrationToken m_controlsUIMessageBrokerToken = {};  // Token for the UI message handler in controls WebView
    EventRegistrationToken m_controlsZoomToken = {};
    EventRegistrationToken m_optionsUIMessageBrokerToken = {};  // Token for the UI message handler in options WebView
    EventRegistrationToken m_optionsZoomToken = {};
    EventRegistrationToken m_lostOptionsFocus = {};  // Token for the lost focus handler in options WebView
    Microsoft::WRL::ComPtr<ICoreWebView2WebMessageReceivedEventHandler> m_uiMessageBroker;

    HRESULT InitUIWebViews();
    HRESULT CreateBrowserControlsWebView();
    HRESULT CreateBrowserOptionsWebView();
    HRESULT ClearContentCache();
    HRESULT ClearControlsCache();
    HRESULT ClearContentCookies();
    HRESULT ClearControlsCookies();

    void SetUIMessageBroker();
    HRESULT ResizeUIWebViews();
    void UpdateMinWindowSize();
    HRESULT PostJsonToWebView(nlohmann::json jsonObj, ICoreWebView2* webview);
    HRESULT SwitchToTab(size_t tabId);
    std::wstring GetFilePathAsURI(std::wstring fullPath);
};
