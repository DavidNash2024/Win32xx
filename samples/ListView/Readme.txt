ListView Example
=============
This project uses a ListView as the view window of a CFrame.
Custom draw is used to modify the text and background colors for the
listview subitems.

This sample also demonstrates how to preform in-place editing of a
list_view control's sub-items. A list-view control provides built in
support for editing the control's labels, but not for editing its 
sub-items. Here we had our own edit control to provide the in-place
editing support.

Features demonstrated in this example
=====================================
* Use of a list-view control as the view window of a frame.
* Use of OnNotifyReflect to handle notifications sent by the list-view in CView.
* Handling NM_CUSTOMDRAW to perform custom drawing in a list-view control.
* Use of CDRF_NOTIFYSUBITEMDRAW to request notifications for drawing sub-items.
* Use of GetItemState and SetItemState to control the list-view's item activation.
* Use of an extra edit control to provide in-place editing support for sub-items.
* Handling the WM_CHAR message of a single line edit control to suppress the beeping
  which occurs when the escape and return keys are pressed.


