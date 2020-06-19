ToolBarDemo Example
===================
This project demonstrates the how use several toolbars in a frame,
and how to customize a ToolBar. A ToolBar is a window control which
has one or more buttons. Clicking these sends a command message to the
parent window, much like selecting a menu item.

In this program we use the window's ToolBar customize feature to
add, move and reposition ToolBar buttons.

The other ToolBar options include:
* Changing the size of the ToolBar images by setting a different image 
   list for the ToolBar.
* Resetting the ToolBar back to its default layout.

The AddToolBarBand function is used to add the additional ToolBars to the
frame in CMainFrame::SetupToolBar. 

There is also a ToolBar displayed over the view window. This ToolBar will
align itself to the Top, Left, Right or Bottom edge depending on which 
button is pressed. This ToolBar is a child window of the view window,
so the code to create and position it is in the View.cpp file.

Note:
Refer to the themes sample for a demonstration of how to replace a 
ToolBar button with a ComboBoxEx control.
  

Features demonstrated in this example
=====================================
* Adding multiple ToolBars to a Frame.
* Customizing the ToolBar using window's ToolBar customize feature.
* Adding and removing buttons from a ToolBar.
* Saving and restoring the ToolBar information.
* Loading Normal, Hot and Disabled image lists for the ToolBar.
* Adding a ToolBar to a view window.
* Building an image list from several icons.
* Displaying a vertical ToolBar.
* Switching a ToolBar from a horizontal to a vertical alignment.







