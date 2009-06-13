Splitter Window
---------------

This program uses the CDocker class to create several splitter windows
inside a window frame.  The four CDocker objects are as follows:
 CPaneBottomLeft
 CPaneBottomRight
 CPaneTopLeft
 CPaneTopRight
 
One of these CDocker objects (CPaneTopLeft) is the view window for the frame.
The remaining CDocker objects are added in CMainFrame::OnInitialUpdate.  
The CDockers use the DS_NO_UNDOCK and DS_NO_CAPTION styles to hide the docker's
caption and supress undocking.

Each CDocker has a view window. The view windows used by the dockers are:
 CViewList
 CViewSimple
 CViewText
 CViewTree

 