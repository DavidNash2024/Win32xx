Titlebar Sample
===============
This sample demonstrates how to customize the title bar of a window in a
manner that is supported on Windows 10 and Windows 11.

The main window with the customized title bar is a "mini frame". It has 
the following features:
* The application's icon is displayed in the title bar. 
* A system menu is displayed when the application's icon is clicked.
* CMenuBar is used to display a menu in the caption area.
* A child view window is positioned over the client area of the main window.


Background
==========
Modern window operating systems (since windows 7) use the Windows Desktop 
Manager (DWM) to manage the the non-client area of top-level windows. This 
impacts on the way we can draw the non-client for these windows. The default
processing of the WM_NCPAINT message has changed, and handling the WM_NCPAINT
message to draw on the non-client area is now deprecated.


About this code
===============
This code uses information published here: 
https://github.com/grassator/win32-window-custom-titlebar
The MIT licence for the original code is included with this software.

This program handles the WM_NCCALCSIZE to remove the title bar from the
window's non-client and add it to the window's client area. We then draw our
custom title bar within the handling of WM_PAINT.  
 

Features demonstrated in this example
=====================================
* Handles the WM_NCCALCSIZE message to adjust the non-client area size.
* Handles the WM_NCHITTEST message for hit testing on our custom title bar.
* Uses ::OpenThemeData to access the windows's theme data.
* Uses ::GetDpiForWindow to retrieve the DPI for the window.
* Uses ::DrawThemeTextEx to draw the tile bar caption.
* Draws a custom title bar with minimize, maximize and close buttons.
* Draws an icon on the title bar.
* Displays a system menu when the icon on the title bar is clicked.
* Positions a child view window over the view area of the window.
* Uses double buffer when drawing the contents of the view window.
* Positions a CMenuBar window on the title bar to display the window menu.
* Uses custom draw to provide the background color for the CMenuBar window.
* Displays the 'About' dialog when F1 is pressed, or from the menu.

