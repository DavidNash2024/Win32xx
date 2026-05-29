EdgeBrowser Sample
==================
This sample demonstrates a simple way to use the Microsoft Edge WebView2
control to display a browser in a window. The WebView2 control uses Microsoft
Edge as the rendering engine to display the web content in native apps.

The EdgeBrowser sample uses packages available from NuGet. Visual Studio's
NuGet Packet Manager can be used to install and update the packages.
The following packages from NuGet are used:
* Microsoft.Web.WebView2
* Microsoft.Windows.Implementation.Library

Features demonstrated in this example
=====================================
* Use of a of a WebView2 control to display a Microsoft Edge browser.
* Use of a simple window to host the WebView2 control.
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
"Get started with WebView2 in Win32 apps" article by Microsoft:
https://learn.microsoft.com/microsoft-edge/webview2/get-started/win32

