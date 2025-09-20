Browser Example
===============
This example demonstrates how to incorporate the use of ATL into a
Win32++ application. Here we use ATL to to create a web browser. We also
use ATL to create an event sink using IDispatch to respond to browser events.

The Active Template Library (ATL) is a set of template-based C++ classes
developed by Microsoft. It simplifies the programming of Component Object Model
(COM) objects.

ATL is provided for free with the later Microsoft compilers. It is an optional
component of the Visual Studio Community compiler which can be added using its
installer. The code in this sample requires ATL version 10 (VS2010) or later.

ATL greatly simplifies the use of COM in our code. I recommend using it if you
are developing your code using COM with a Microsoft compiler. ATL is only
supported on Microsoft compilers though. It is not provided with MinGW or 
modern Embacardero C++ compilers.


Features demonstrated in this sample
====================================
* Use of a ComboBoxEx control within the frame's rebar.
* Use of Active Template Library (ATL) to incorporate ActiveX components.
* Use of the IWebBrowser2 interface to render a web page.
* Use of IDispatch to create an event sink for IWebBrowser2 events.

Note:
This sample uses the IWebBrowser2 interface to display web pages. The IWebBrowser2
provides access to the same features as used by the Microsoft's Internet Explorer.
Support for Internet Explorer ended on June 15, 2022, so this sample might not render
web pages properly.

Consider using using the EdgeBrowser and EdgeBrowserEx samples to display modern
web pages instead.
