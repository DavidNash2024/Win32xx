Tray Example
==============
This project demonstrates how to use to create an application which can be
minimised to the tray icon, and restored.


Features demonstrated in this example
=====================================
* Inheriting from CWinApp to start Win32++
* Inheriting from CWnd to create a view window
* Using a resource script (resource.rc) to add an icon and window caption
* Processing window messages in CView::WndProc
* Overriding PreCreate to set the initial window parameters
* Overriding OnPaint to draw text on the window's device context
* Using TRACE to ouput debug text