Custom Control Example
==================
This is sample demonstrates how to build a custom control and display it
in a dialog. The custom control is created from WebView2

This sample uses packages available from NuGet. Visual Studio's NuGet Packet
Manager can be used to install and update the packages.
The following packages from NuGet are used:
* Microsoft.Web.WebView2
* Microsoft.Windows.Implementation.Library

Features demonstrated in this example
=====================================
* Registering a custom control.
* Specifying the custom control in resource.rc.
* Using WebView2 edge browser in a custom control.
* Automatic resizing of the custom control when the dialog is resized

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

