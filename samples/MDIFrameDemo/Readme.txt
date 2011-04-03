MDIFrameDemo Example
====================
This project demonstrates the use of the CMDIFrame class to display a MDI frame
window. MDI (Multiple Document Interface) frames allow different types of view
windows to be displayed simultaneously. The more common SDI (Single Document 
Interface) frames only display a single view.

In this project we have the following different types of MDI children available:
 - A simple view window which displays some text
 - A window which will be initally displayed as maximised
 - A window displaying rectangles drawn on the device context
 - A RichText view window
 - A ListView view window
 - A Treeview view window
 
Classes titled CMDIChildXXXX are inherited from CMDIChildr. Classes titled 
CViewXXXX are the various view windows inherited from CWnd.


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
* Setting the seperate menu for the CMDIChild.
* Reponding to the MDI specific commands in CMDIFrame::OnCommand
* Using several different types of MDI children
   - Each MDI child type has a different menu and icon
   - Each MDI child type has a differnet view window
* Handling a MDI child's OnClose to provide an opportunity to save data before
   the MDI child's window is closed.   








