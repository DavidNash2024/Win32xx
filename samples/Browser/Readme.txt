Browser Example
===============
This example demonstrates how to use CWebBrowser as a view window for a frame.
It also demonstrates how to implement an EventSink to allow the WebBrowser
to provide notifications of web browsing events.

The Rebar control includes a ComboBoxEx control which allows URLs to be
entered for web browsing.

This program support print and print preview.

Features demonstrated in this example
=====================================
* Use of CFrame to display the window frame.
* Use of CWebBrowser as a view window for the frame.
* Use of an event sink to get notification of browser events.
* Use of 32bit bitmaps for the toolbar.
* Use of hot and disabled image lists for the toolbar.
* Adding icons for popup menu items.
* Use of a ComboBoxEx control in the rebar.
* Printing the contents of a WebBrowser.
* Print preview the contents of a WebBrowser.

Note:
The CWebBrowser used in this sample uses the IWebBrowser2 interface. The IWebBrowser2
provides access to the same features as used by the Microsoft's Internet Explorer.
Support for Internet Explorer ended on June 15, 2022, so this sample might not render
web pages properly.

Consider using using the EdgeBrowser and EdgeBrowserEx samples to display modern
web pages instead.
