TabDemo Example
===============
This project demonstrates how to use the CTab control within a frame.

CTab is per monitor dpi aware. The tab sizes, tab font, and tab icon sizes
are automatically adjusted when a tab window is moved between monitors with
different display settings.
 
CTab also supports rendering the tabs with or without owner-draw. When
owner-draw is used, CTab can also display a tab-list button and a close button. 

The options that can be selected using the menu are:
* Enable or disable owner draw.
* Enable or disable fixed width tabs when owner-draw is not used.
* Show or hide the tab's tab-list and close buttons when owner-draw is used.
* Show the tabs at the top or bottom of the view.

Refer also to the DialogTab sample which demonstrates using a CTab in a dialog.

Features demonstrated in this example
=====================================
* Use of CFrame to display the window frame, with a tab control as its view.
* Displaying tabs at the top or bottom of the tab control.
* How to Display/Hide the tab close and tab list buttons.
* Use of OnMenuUpdate to check or uncheck and enable or disable menu items.
* Displaying larger fonts and icons in the tabs.

