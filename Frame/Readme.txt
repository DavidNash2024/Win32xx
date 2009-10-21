Frame Sample
============
This project demonstrates the use of the CFrame class to display a frame
window. A frame uses a seperate view window which is displayed over its 
client area.

CFrame provides a set of features include:
- a toolbar 
- a Menubar
- a rebar to hold the frame's toolbar and menubar
- a statusbar, displaying toolbar/menubar status and indicators
- support for a seperate view window
- tooltips for toolbars
- themes support to set colours for the rebar, menubar and toolbar
- Most Recently Used (MRU) list support
- The ability to save its settings in the registry 


This is a very simple project which could be used as a starting point for your
own frame based applications.


Features demonstrated in this sample
====================================
* Use of CFrame to display the window frame
* Toolbar configuration
* Enabling/Disabling various frame features in OnCreate, namely:
   - show/hide toolbar and menu status in the status bar.
   - show/hide indicators in the status bar.
   - Use/don't use a rebar in the frame.
   - Use/don't use themes.
   - Use/don't use a toolbar in the frame.
* Setting the view window of the frame.
* Responding to toolbar and menu commands in OnCommand
* Responding to notifications in OnNotify.
* Use of OpenFile and print dialogs.
* Responding to frame window messages in CFrame::WndProc
* Responding to view window messages in CView::WndProc
* Using the TRACE command to ouput debug text
* Saving the frame settings in the registry






