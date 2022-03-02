Splitter Window
===============
This program uses the CDocker class to create several splitter windows
inside a window frame.  The four CDocker objects are as follows:
 CPaneBottomLeft
 CPaneBottomRight
 CPaneTopLeft
 CPaneTopRight
 
CMainFrame inherits from CDockFrame to include the splitter window support.
Splitter windows is a subset of docking the capabilities. 
 
One of these CDocker objects (CPaneTopLeft) is the view window for the frame.
The remaining CDocker objects are added in CMainFrame::OnInitialUpdate.  
The CDockers use the DS_NO_UNDOCK and DS_NO_CAPTION styles to hide the docker's
caption and suppress undocking.

Each CDocker has a view window. The view windows used by the dockers are:
 CViewList
 CViewSimple
 CViewText
 CViewTree


Features demonstrated in this example
=====================================
* Use of CDockFrame to provide a frame window
* Use of CDocker to display window panes separated by a splitter bar.
* Use of ListView, TreeView and Edit controls as view windows
* The storing of the window pane arrangement in the registry.
 