EdgeBrowserEx Sample
====================
This sample uses the Microsoft Edge WebView2 control to display a tabbed 
browser in a window. The WebView2 control is used to manage the tabs and
display the Microsoft Edge browser.


Features demonstrated in this example
=====================================
* Use of a WebView2 control to manage tabs.
* Use of a WebView2 control to display a Microsoft Edge browser within a tab.
* Handling messages from the WebView2 tab control.
* Handling messages from the WebView2 browser.
* Storing and managing Favourites
* Storing and managing History
* Implementing a settings menu for clearing the cache and cookies.
* Passing JSON objects to and from the web view (ICoreWebView2).
* Use of 'JSON for Modern C++' to parse between JSON and C++.
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


About this sample
=================
This sample was adapted from the information provided in the
"Win32 sample WebView2Browser" article by Microsoft:
https://learn.microsoft.com/en-gb/microsoft-edge/webview2/samples/webview2browser
https://github.com/MicrosoftEdge/WebView2Browser


About the cpprestsdk library used in the Microsoft sample
=========================================================
Microsoft advises that cpprestsdk is in maintenance mode as is not recommended
for use in new projects. The cpprestsdk library is not supported by Visual 
Studio 2022 v143 Platform Toolset.

This Win32++ sample uses the 'JSON for Modern C++' library by Niels Lohmann in
place of the cpprsetsdk library.
