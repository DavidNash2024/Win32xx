Round Window Example
====================
This project demonstrates how to use Win32++ to create a round window. A
circular region defines the shape of the window.

A right mouse button click brings up a popup menu to set the window's 
background color or close the window.

The window can be moved with click and drag.


Features demonstrated in this example
=====================================
* Inheriting from CWinApp to start Win32++
* Inheriting from CWnd to create a view window
* Using a resource script (resource.rc) to add an icon and window caption
* Processing window messages in CView::WndProc
* Overriding PreCreate to set the initial window creation parameters
* Overriding PreRegister class to set the window class parameters
* Using a circular region to define the shape of the window.
* Creating a popup menu in response to a right mouse button click.
* Overriding OnDraw to draw text on the window's device context
* Using the ChooseColor dialog to set the window's background window.