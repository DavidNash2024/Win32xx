DarkModeMiniFrame Sample
========================
This sample combines the features presented in the MiniFrame sample
and the DarkModeFrame sample. It displays a frame with a modified titlebar.
The color of the titlebar is configurable. The color chosen for the titlebar
matches the background theme color used for the frame's rebar, or black for
dark mode.

We can also choose to whether or not to display the menubar in the title bar.


 We can also choose to whether
or not to display the menubar in the title bar.

The application also detects if dark mode for applications is enabled and
automatically adjusts its theme accordingly. 

This code requires Visual Studio Community 2022 with the Windows App SDK
installed. The Windows App SDK can be installed using the VS2022 installer
as follows:
* Select Desktop development with C++.
* Then in the Installation details pane of the installation dialog box, 
  select Windows App SDK C++ Templates (at the bottom of the list).

More detailed instructions can be found on the following web site.
https://learn.microsoft.com/en-us/windows/apps/windows-app-sdk/set-up-your-development-environment

Some of the code in this sample is based on code provided here:
https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/apply-windows-themes


The frame produced by this sample can be presented in either of two modes.

The full-frame mode has the following features:
 - The caption is displayed in the frame's titlebar.
 - The menu is displayed by CMenubar within the rebar control.
 - The toolbar is displayed within the rebar control.

The mini-frame mode has the following features:
 - The menu is displayed by CMenubar in the frame's titlebar.
 - The toolbar is displayed within the rebar control.


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
 

This sample demonstrates the following:
======================================
* Handles the WM_NCCALCSIZE message to adjust the non-client area size.
* Handles the WM_NCHITTEST message for hit testing on our custom title bar.
* Handles the WM_GETMINMAXINFO message to limit the minimum window size.
* Uses ::OpenThemeData to access the windows's theme data.
* Uses ::GetDpiForWindow to retrieve the DPI for the window.
* Uses ::DrawThemeTextEx to draw the tile bar caption.
* Draws a custom title bar with minimize, maximize and close buttons.
* Draws an icon on the title bar.
* Displays a system menu when the icon on the title bar is clicked.
* Positions a child view window over the view area of the window.
* Uses double buffer when drawing the contents of the view window.
* Overrides CFrame<T>::RecalcLayout to reposition the frame's child windows.
* Adjusts the position of the frame's rebar control.
* Overrides GetViewRect to adjust the frame's view area.
* Uses CRebar::ShowBand to dynamically show or hide a rebar band.


