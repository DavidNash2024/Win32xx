EdgeBrowser Sample
==================
This sample uses the Microsoft Edge WebView2 control to display a browser in a
window. The WebView2 control uses Microsoft Edge as the rendering engine to
display the web content in native apps.


Features demonstrated in this example
=====================================
* Use of a of a WebView2 control to display a Microsoft Edge browser.
* Use of a simple window to host the WebView2 control.
* Use of Per Monitor DPI Version 2 added as manifest resource.


About the WebView2 Runtime.
===========================
The WebView2 Runtime is required to run applications using the WebView2 control.
The Evergreen Runtime is preinstalled onto all Windows 11 devices as a part of
the Windows 11 operating system. Microsoft installed the WebView2 Runtime to all
eligible Windows 10 devices, as described in Delivering Microsoft Edge WebView2
Runtime to managed Windows 10 devices. Even if your app uses the Evergreen
distribution mode, Microsoft recommends that you distribute the WebView2 Runtime,
to cover edge cases where the Runtime wasn't already installed.

WebView2 Runtime version 109 is the final version that supports the following
versions of Windows. WebView2 Runtime and SDK version 110.0.1519.0 and higher
don't support these operating systems.
    Windows 8/8.1
    Windows 7
    Windows Server 2012 R2
    Windows Server 2012
    Windows Server 2008 R2


Refer here for information on distributing the runtime with your application:
https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/distribution


Refer her for additional information on WebView2:
https://learn.microsoft.com/en-us/microsoft-edge/webview2/

