DockMDIFrame Example
====================
This project demonstrates the use of docking in combination with a standard 
MDI frame window. MDI (Multiple Document Interface) frames allow different 
types of view windows to be displayed simultaneously. The more common SDI
(Single Document Interface) frames only display a single view.

CMainMDIFrame inherits from CMDIDockFrame to enable docking support.

Caveat
======
Multiple-document interface (MDI) applications are officially discouraged
by Microsoft. They do not render properly with Per-Monitor (V2) DPI Awareness.

Win32++ provides the CTabbedMDI class which can be used as an alternative to
MDI applications.

Features demonstrated in this example
=====================================
* Use of CMDIDockFrame to implement a MDI docking frame.
* Adding other dockers to the MDI docking frame.
* Adding MDI children to the MDI docking frame.
* Enabling/Disabling various frame features in OnCreate, namely:
   - show/hide toolbar and menu status in the status bar.
   - show/hide indicators in the status bar.
   - Use/don't use a rebar in the frame.
   - Use/don't use themes.
   - Use/don't use a toolbar in the frame.
* Setting the MDI Frame's toolbar.  
* Setting the view window for the CMDIChid.
* Setting the separate menu for the CMDIChild.
* Responding to the MDI specific commands such as cascade and tile in OnCommand.

