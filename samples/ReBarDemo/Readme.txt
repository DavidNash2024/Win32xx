ReBarDemo Example
=================
This project demonstrates the how to add a second ReBar to a frame application.
The ReBar can be positioned at the top, left right or bottom of the frame.

The ReBar contains a ToolBar. Pressing the the ToolBar buttons (left, right, 
top or bottom) reposition the ReBar.

The additional ReBar is a child window of the frame. It is created in 
CMainFrame::OnCreate. CMainFrame::RecalLayout manages the position of all the
frame's child windows including the new ReBar.  

Features demonstrated in this example
=====================================
* Creating and positioning a ReBar control.
* Adding a ToolBars to the ReBar control.
* Changing a ToolBar from horizontal to vertical.
* Changing a ReBar from horizontal to vertical.
* Overriding the following functions
  - CFrame::GetViewRect
  - CFrame::OnCreate
  - CFrame::RecalcLayout 
   







