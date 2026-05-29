/////////////////////////////
// EdgeView.h
//

#ifndef _EDGEVIEW_H_
#define _EDGEVIEW_H_

// Include the WebView2 Runtime files from the packages folder.
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"


class CEdgeView : public CWnd
{
public:
    BOOL CanGoBack();
    BOOL CanGoForward();
    void GoBack();
    void GoForward();
    void Navigate(LPCWSTR url);
    void Reload();
    void SetupEvents(HWND hWnd);
    void StartBrowser();
    void Stop();

protected:
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    wil::com_ptr<ICoreWebView2Controller> m_controller;
    wil::com_ptr<ICoreWebView2> m_webView;
};

#endif // _EDGEVIEW_H_
