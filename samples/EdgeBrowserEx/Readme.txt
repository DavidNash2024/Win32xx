EdgeBrowserEx Sample
====================
This sample uses the Microsoft Edge WebView2 control to display a tabbed 
browser in a window. The WebView2 control is used to manage the tabs and
display the Microsoft Edge browser.

This browser sample uses packages available from NuGet. Visual Studio's NuGet
Packet Manager can be used to install and update the packages.
The following packages from NuGet are used:
* Microsoft.Web.WebView2
* Microsoft.Windows.Implementation.Library

This sample also uses the 'JSON for Modern C++' library by Niels Lohmann.

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
* Adding support for Per Monitor DPI Version 2 in a manifest resource.

About WebView2
==============
Microsoft's Edge WebView2 is a system component that allows developers to embed
web content (HTML, CSS, and JavaScript) directly into native Windows
applications using the Microsoft Edge (Chromium) rendering engine. The Edge
WebView2 Control is the UI component embedded by developers.

Refer here for additional information on WebView2:
https://learn.microsoft.com/en-us/microsoft-edge/webview2/

About the WebView2 Runtime.
===========================
The WebView2 Runtime is required to run applications using the WebView2 control.
The Evergreen Runtime is preinstalled onto all Windows 11 devices as a part of
the Windows 11 operating system. The Microsoft Edge WebView2 Runtime is
preinstalled on the vast majority of internet-connected Windows 10 devices
running version 2101 or later.

Refer here for information on distributing the runtime with your application:
https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/distribution

About this sample
=================
This sample was adapted from the information provided in the
"Win32 sample WebView2Browser" article by Microsoft:
https://learn.microsoft.com/en-gb/microsoft-edge/webview2/samples/webview2browser
https://github.com/MicrosoftEdge/WebView2Browser


About the cpprestsdk library used in the Microsoft sample
=========================================================
Microsoft advises that cpprestsdk is in maintenance mode as is not recommended
for use in new projects. This Win32++ sample uses the 'JSON for Modern C++'
library by Niels Lohmann in place of the cpprsetsdk library.
