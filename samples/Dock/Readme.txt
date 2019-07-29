Dock Example
============
This example demonstrates simple docking (docking without containers). A 
collection of different "view windows" are used by the CDocker class.

Dockers are capable of allowing other Dockers to dock to and undock from them. 
When docking, the undocked Docker is dragged over another Docker.  Various 
visual clues such has the dock targeting (small arrow-like images), and the 
dock hinting (where a portion of the destination window turns blue) provide a 
hint as to where the Docker will be docked.  To facilitate undocking, the 
caption of the docked window is dragged and dropped.

Every Docker has a view window.  These views can be any child window, and are 
set in the same way as views for Frames and MDI children.  Containers 
(provided by the CDockContainer class) are a specialized view which add 
additional docking features when used as the view window for a Docker.

Classes titled CDockXXXX are inherited from CDocker. Classes titled CViewXXXX
are the various view windows inherited from CWnd.

CMainFrame inherits from CDockFrame. CDockFrame adds support for docking
to CMainFrame. 

Refer to the documentation for further information on the use of the CDocker
class.


Features demonstrated in this example
=====================================
* Use of CDockFrame to provide a frame window.
* Use of CDockFrame to provide docking support.
* Use of RichEdit, ListView and TreeView windows as view windows for CDocker.
* Saving the dock layout in the registry.
* Use of OnMenuUpdate to manage menu item check boxes
* Demonstrates the effects of the various dock styles
