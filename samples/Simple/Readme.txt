Simple Example
==============
This project demonstrates how to use Win32++ to create a simple window.  The
CView class controls the view window. The CSimpleApp starts Win32++ and runs
the message loop.


Features demonstrated in this example
=====================================
* Inheriting from CWinApp to start Win32++
* Inheriting from CWnd to create a view window
* Using a resource script (resource.rc) to add an icon and window caption
* Processing window messages in CView::WndProc
* Overriding PreCreate to set the initial window parameters
* Overriding OnDraw to draw text on the window's device context
* Using TRACE to output debug text