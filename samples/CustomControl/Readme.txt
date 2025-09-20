Custom Control Example
==================
This is sample demonstrates how to build a custom control and display it
in a dialog. The custom control is created from CWebBrowser.


Features demonstrated in this example
=====================================
* Registering a custom control.
* Specifying the custom control in resource.rc.
* Using CWebBrowser in a custom control.
* Automatic resizing of the custom control when the dialog is resized

Note:
This sample uses the IWebBrowser2 interface to display web pages. The IWebBrowser2
provides access to the same features as used by the Microsoft's Internet Explorer.
Support for Internet Explorer ended on June 15, 2022, so this sample might not render
web pages properly.

Consider using using the EdgeBrowser and EdgeBrowserEx samples to display modern
web pages instead.
