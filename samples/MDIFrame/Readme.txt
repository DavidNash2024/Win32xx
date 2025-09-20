MDIFrame Example
================
This project demonstrates the use of the CMDIFrame class to display a MDI frame
window. MDI (Multiple Document Interface) frames allow different types of view
windows to be displayed simultaneously. The more common SDI (Single Document 
Interface) frames only display a single view.

The menu for a MDI frame should have a popup menu item called "Window"
positioned second from the right. An entry for each MDI child will be appended
to this menu item. It a MDI child have a menu, this should have a popup menu
item called "Window" positioned second from the right as well.

This is a very simple project which could be used as a starting point for your
own MDI frame based applications.


Caveat
======
Multiple-document interface (MDI) applications are officially discouraged
by Microsoft. They do not render properly with Per-Monitor (V2) DPI Awareness.

Win32++ provides the CTabbedMDI class which can be used as an alternative to
MDI applications.


Features demonstrated in this example
=====================================
* Use of CMDIFrame and CMDIChild classes to implement a MDI frame
* Enabling/Disabling various frame features in OnCreate, namely:
   - show/hide toolbar and menu status in the status bar.
   - show/hide indicators in the status bar.
   - Use/don't use a rebar in the frame.
   - Use/don't use themes.
   - Use/don't use a toolbar in the frame.
* Setting the MDI Frame's toolbar.  
* Setting the view window for the CMDIChid.
* Setting the separate menu for the CMDIChild.
* Responding to the MDI specific commands in CMDIFrame::OnCommand
