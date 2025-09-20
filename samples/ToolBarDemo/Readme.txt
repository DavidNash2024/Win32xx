ToolBarDemo Example
===================
This project demonstrates the how use several toolbars in a frame, and how to
customize a toolbar. A toolbar is a window control that has one or more
buttons. Clicking these sends a command message to the parent window, much like
selecting a menu item.

In this program we use the window's toolbar customize feature to add, move and
reposition toolbar buttons.

The other toolbar options include:
* Changing the size of the toolbar images by setting a different image list for
  the toolbar.
* Resetting the toolbar back to its default layout.

The AddToolBarBand function is used to add the additional toolbars to the frame
in CMainFrame::SetupToolBar. 

There is also a toolbar displayed over the view window. This toolbar will align
itself to the Top, Left, Right or Bottom edge depending on which button is
pressed. This toolbar is a child window of the view window, so the code to
create and position it is in the View.cpp file.


Features demonstrated in this example
=====================================
* Adding multiple toolbars to a Frame.
* Customizing the toolbar using window's toolbar customize feature.
* Adding and removing buttons from a toolbar.
* Saving and restoring the toolbar information.
* Loading Normal, Hot and Disabled image lists for the toolbar.
* Adding a toolbar to a view window.
* Building an image list from several icons.
* Displaying a vertical toolbar.
* Switching a toolbar from a horizontal to a vertical alignment.







