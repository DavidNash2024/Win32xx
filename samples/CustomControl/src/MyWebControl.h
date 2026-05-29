/////////////////////////////
// MyWebControl.h
//

#ifndef __MYCONTROL_H
#define __MYCONTROL_H

// Include the WebView2 Runtime files from the packages folder.
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"


/////////////////////////////////////////////////
// CWebControl is a custom control for the dialog
// that displays a web browser.
class CWebControl : public CWnd
{
public:
    CWebControl();
    virtual ~CWebControl() override = default;

    void StartBrowser();

protected:
    virtual LRESULT OnDPIChanged(UINT, WPARAM, LPARAM lparam);
    virtual void OnInitialUpdate() override;
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CWebControl(const CWebControl&) = delete;
    CWebControl& operator=(const CWebControl&) = delete;

    wil::com_ptr<ICoreWebView2Controller> m_controller;
    wil::com_ptr<ICoreWebView2> m_webView;
};


#endif // __MYCONTROL_H

